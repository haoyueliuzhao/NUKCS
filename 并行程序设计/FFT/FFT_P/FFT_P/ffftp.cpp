#include <iostream>
#include <vector>
#include <algorithm>
#include <complex>
#include <cmath>
#include <chrono>
#include <pthread.h> // 包含pthread支持

using namespace std;
using namespace std::chrono;

const double pi = 3.1415926;

// 线程数据结构
struct ThreadData {
    vector<complex<double>>* input;
    vector<complex<double>>* output;
    int size;
};

// FFT函数
void FFT(int x, vector<complex<double>>& A, vector<complex<double>>& fft)
{
    if (x == 1)
    {
        fft[0] = A[0];
        return;
    }
    vector<complex<double>> A_even(x / 2, 0);
    vector<complex<double>> A_odd(x / 2, 0);
    for (int i = 0; i < x / 2; i++)
    {
        A_even[i] = A[i * 2];
        A_odd[i] = A[i * 2 + 1];
    }
    vector<complex<double>> fft_e(x / 2);
    vector<complex<double>> fft_o(x / 2);
    FFT(x / 2, A_even, fft_e);
    FFT(x / 2, A_odd, fft_o);

    for (int k = 0; k < x / 2; k++)
    {
        complex<double> Wk(cos(2 * pi * k / x), sin(2 * pi * k / x));
        fft[k] = fft_e[k] + Wk * fft_o[k];
        fft[k + x / 2] = fft_e[k] - Wk * fft_o[k];
    }
}

// IFFT函数
void IFFT(int x, vector<complex<double>>& B, vector<complex<double>>& ifft)
{
    if (x == 1)
    {
        ifft[0] = B[0];
        return;
    }
    vector<complex<double>> B_even(x / 2, 0);
    vector<complex<double>> B_odd(x / 2, 0);
    for (int i = 0; i < x / 2; i++)
    {
        B_even[i] = B[i * 2];
        B_odd[i] = B[i * 2 + 1];
    }
    vector<complex<double>> ifft_e(x / 2);
    vector<complex<double>> ifft_o(x / 2);
    IFFT(x / 2, B_even, ifft_e);
    IFFT(x / 2, B_odd, ifft_o);
    for (int k = 0; k < x / 2; k++)
    {
        complex<double> Wk(cos(-2 * pi * k / x), sin(-2 * pi * k / x));
        ifft[k] = (ifft_e[k] + Wk * ifft_o[k]);
        ifft[k + x / 2] = (ifft_e[k] - Wk * ifft_o[k]);
    }
}

// FFT或IFFT的线程函数
void* fft_thread(void* arg)
{
    ThreadData* data = (ThreadData*)arg;
    FFT(data->size, *data->input, *data->output);
    return NULL;
}

void* ifft_thread(void* arg)
{
    ThreadData* data = (ThreadData*)arg;
    IFFT(data->size, *data->input, *data->output);
    return NULL;
}

int main()
{
    const int numExperiments = 5; // 每个规模的实验次数
    const int initialSize = 128;
    int n = 64;
    int m = 64;

    for (int i = 0; i < 10; i++)
    {
        n = n * 2;
        m = m * 2;
        int y = 1;

        // 计算y，使其为2的幂次
        while (y < n + m + 1)
        {
            y *= 2;
        }

        double totalTime = 0.0;

        for (int j = 0; j < numExperiments; j++)
        {
            // 生成随机测试用例
            vector<complex<double>> N;
            vector<complex<double>> M;
            srand(time(0)); // 随机数种子
            for (int k = 0; k <= n; k++)
            {
                int x = rand() % 10 + 1;
                complex<double> temp(x, 0);
                N.push_back(temp);
            }
            reverse(N.begin(), N.end());
            for (int k = n + 1; k < y; k++)
            {
                N.push_back({ 0, 0 });
            }

            vector<complex<double>> fft_N(y);

            auto startFFT = high_resolution_clock::now();

            // 使用pthread并行化FFT
            pthread_t threads[2];
            ThreadData data1 = { &N, &fft_N, y };
            pthread_create(&threads[0], NULL, fft_thread, (void*)&data1);

            for (int k = 0; k <= m; k++)
            {
                int x = rand() % 10 + 1;
                complex<double> temp(x, 0);
                M.push_back(temp);
            }
            reverse(M.begin(), M.end());
            for (int k = m + 1; k < y; k++)
            {
                M.push_back({ 0, 0 });
            }
            vector<complex<double>> fft_M(y);

            ThreadData data2 = { &M, &fft_M, y };
            pthread_create(&threads[1], NULL, fft_thread, (void*)&data2);

            for (int t = 0; t < 2; ++t) {
                pthread_join(threads[t], NULL);
            }

            auto endFFT = high_resolution_clock::now();
            duration<double> timeFFT = endFFT - startFFT;

            vector<complex<double>> fft_Result(y);
            for (int k = 0; k < y; k++)
            {
                fft_Result[k] = fft_N[k] * fft_M[k];
            }

            vector<complex<double>> Result(y);

            auto startIFFT = high_resolution_clock::now();

            // 使用pthread并行化IFFT
            pthread_t thread;
            ThreadData ifft_data = { &fft_Result, &Result, y };
            pthread_create(&thread, NULL, ifft_thread, (void*)&ifft_data);
            pthread_join(thread, NULL);

            auto endIFFT = high_resolution_clock::now();
            duration<double> timeIFFT = endIFFT - startIFFT;

            totalTime += (timeFFT + timeIFFT).count();
        }

        double averageTime = totalTime / numExperiments;
        cout << "n=" << n << ", m=" << m << endl;
        cout << "平均总时间: " << averageTime * 1000 << " 毫秒" << endl;
        cout << endl;
    }

    return 0;
}
