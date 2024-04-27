#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;
const int N = 260; // 定义矩阵的大小

vector<vector<double>> m(N, vector<double>(N, 0.0));

void m_reset() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            m[i][j] = 0;
        }
        m[i][i] = 1.0;
        for (int j = i + 1; j < N; ++j) {
            m[i][j] = static_cast<double>(rand()) / RAND_MAX; // 随机生成一个0到1之间的浮点数
        }
    }
    for (int k = 0; k < N; ++k) {
        for (int i = k + 1; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                m[i][j] += m[k][j];
            }
        }
    }
}
void Gaussian_Elimination(vector<vector<double>>& A, vector<double>& b) {
    int n = A.size();

    // 消元过程
    for (int k = 0; k < n; ++k) {
        for (int i = k + 1; i < n; ++i) {
            double factor = A[i][k] / A[k][k];
            for (int j = k + 1; j < n; ++j) {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
        }
    }

    // 回代过程
    vector<double> x(n);
    x[n - 1] = b[n - 1] / A[n - 1][n - 1];
    for (int i = n - 2; i >= 0; --i) {
        double sum = b[i];
        for (int j = i + 1; j < n; ++j) {
            sum -= A[i][j] * x[j];
        }
        x[i] = sum / A[i][i];
    }

    // 打印解向量
    /*cout << "解向量 x:" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "x[" << i << "] = " << x[i] << endl;
    }*/
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    m_reset();
    vector<double> b;
    for (int i = 0; i < N; i++)
    {
        b.push_back(static_cast<double>(rand()));
    }
    vector<int>time;
    int x = 5;
    for (int i = 0; i < x; i++)
    {
        auto start = high_resolution_clock::now(); // 获取当前时间点
        Gaussian_Elimination(m, b);
        auto end = high_resolution_clock::now(); // 获取当前时间点
        auto duration = duration_cast<nanoseconds>(end - start); // 计算时间差
        time.push_back(duration.count());
        //cout << "执行时间: " << duration.count() << " ns" << endl;
        
    }
    int sum=0;
    for (int& i : time)
    {
        cout << "执行时间: " << i << " ns" << endl;
        sum += i;
    }
    cout << "平均执行时间：" << sum / x << "ns" << endl;
    return 0;
}
