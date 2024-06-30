#include <cuda_runtime.h>
#include <cuComplex.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>
#include <corecrt_math_defines.h>
#include <device_launch_parameters.h>

using namespace std;
using namespace std::chrono;

// CUDA 核函数来计算 FFT
__global__ void fftKernel(cuDoubleComplex* d_A, cuDoubleComplex* d_fft, int x) {
    extern __shared__ cuDoubleComplex shared[];
    int k = blockIdx.x * blockDim.x + threadIdx.x;
    if (k < x) {
        cuDoubleComplex sum = make_cuDoubleComplex(0.0, 0.0);
        for (int n = 0; n < x; ++n) {
            double theta = -2.0 * M_PI * k * n / x;
            cuDoubleComplex exp = make_cuDoubleComplex(cos(theta), sin(theta));
            sum = cuCadd(sum, cuCmul(d_A[n], exp));
        }
        d_fft[k] = sum;
    }
}

// CUDA 核函数来计算 IFFT
__global__ void ifftKernel(cuDoubleComplex* d_B, cuDoubleComplex* d_ifft, int x) {
    extern __shared__ cuDoubleComplex shared[];
    int k = blockIdx.x * blockDim.x + threadIdx.x;
    if (k < x) {
        cuDoubleComplex sum = make_cuDoubleComplex(0.0, 0.0);
        for (int n = 0; n < x; ++n) {
            double theta = 2.0 * M_PI * k * n / x;
            cuDoubleComplex exp = make_cuDoubleComplex(cos(theta), sin(theta));
            sum = cuCadd(sum, cuCmul(d_B[n], exp));
        }
        d_ifft[k] = make_cuDoubleComplex(cuCreal(sum) / x, cuCimag(sum) / x);
    }
}

int main() {
    const int numExperiments = 5; // 每个规模的实验次数
    const int initialSize = 128;
    int n = 64;
    int m = 64;

    for (int i = 0; i < 10; i++) {
        n = n * 2;
        m = m * 2;
        int y = 1;

        // 计算y，使其为2的幂次
        while (y < n + m + 1) {
            y *= 2;
        }

        double totalTime = 0.0;

        for (int j = 0; j < numExperiments; j++) {
            // 生成随机测试用例
            vector<cuDoubleComplex> N;
            vector<cuDoubleComplex> M;
            srand(time(0)); // 随机数种子
            for (int k = 0; k <= n; k++) {
                int x = rand() % 10 + 1;
                cuDoubleComplex temp = make_cuDoubleComplex(x, 0);
                N.push_back(temp);
            }
            reverse(N.begin(), N.end());
            for (int k = n + 1; k < y; k++) {
                N.push_back(make_cuDoubleComplex(0, 0));
            }

            for (int k = 0; k <= m; k++) {
                int x = rand() % 10 + 1;
                cuDoubleComplex temp = make_cuDoubleComplex(x, 0);
                M.push_back(temp);
            }
            reverse(M.begin(), M.end());
            for (int k = m + 1; k < y; k++) {
                M.push_back(make_cuDoubleComplex(0, 0));
            }

            // 分配设备内存
            cuDoubleComplex* d_N;
            cuDoubleComplex* d_M;
            cuDoubleComplex* d_fft_N;
            cuDoubleComplex* d_fft_M;
            cuDoubleComplex* d_fft_Result;
            cuDoubleComplex* d_Result;

            cudaMalloc((void**)&d_N, y * sizeof(cuDoubleComplex));
            cudaMalloc((void**)&d_M, y * sizeof(cuDoubleComplex));
            cudaMalloc((void**)&d_fft_N, y * sizeof(cuDoubleComplex));
            cudaMalloc((void**)&d_fft_M, y * sizeof(cuDoubleComplex));
            cudaMalloc((void**)&d_fft_Result, y * sizeof(cuDoubleComplex));
            cudaMalloc((void**)&d_Result, y * sizeof(cuDoubleComplex));

            // 复制数据到设备
            cudaMemcpy(d_N, N.data(), y * sizeof(cuDoubleComplex), cudaMemcpyHostToDevice);
            cudaMemcpy(d_M, M.data(), y * sizeof(cuDoubleComplex), cudaMemcpyHostToDevice);

            auto startFFT = high_resolution_clock::now();

            // 执行 FFT 核函数
            int threadsPerBlock = 256;
            int blocksPerGrid = (y + threadsPerBlock - 1) / threadsPerBlock;
            fftKernel << <blocksPerGrid, threadsPerBlock, y * sizeof(cuDoubleComplex) >> > (d_N, d_fft_N, y);
            fftKernel << <blocksPerGrid, threadsPerBlock, y * sizeof(cuDoubleComplex) >> > (d_M, d_fft_M, y);
            cudaDeviceSynchronize();

            auto endFFT = high_resolution_clock::now();
            duration<double> timeFFT = endFFT - startFFT;

            // 复制 FFT 结果回主机
            vector<cuDoubleComplex> fft_N(y);
            vector<cuDoubleComplex> fft_M(y);
            cudaMemcpy(fft_N.data(), d_fft_N, y * sizeof(cuDoubleComplex), cudaMemcpyDeviceToHost);
            cudaMemcpy(fft_M.data(), d_fft_M, y * sizeof(cuDoubleComplex), cudaMemcpyDeviceToHost);

            // 计算结果的点乘
            vector<cuDoubleComplex> fft_Result(y);
            for (int k = 0; k < y; k++) {
                fft_Result[k] = cuCmul(fft_N[k], fft_M[k]);
            }

            // 复制点乘结果到设备
            cudaMemcpy(d_fft_Result, fft_Result.data(), y * sizeof(cuDoubleComplex), cudaMemcpyHostToDevice);

            auto startIFFT = high_resolution_clock::now();

            // 执行 IFFT 核函数
            ifftKernel << <blocksPerGrid, threadsPerBlock, y * sizeof(cuDoubleComplex) >> > (d_fft_Result, d_Result, y);
            cudaDeviceSynchronize();

            auto endIFFT = high_resolution_clock::now();
            duration<double> timeIFFT = endIFFT - startIFFT;

            // 复制 IFFT 结果回主机
            vector<cuDoubleComplex> Result(y);
            cudaMemcpy(Result.data(), d_Result, y * sizeof(cuDoubleComplex), cudaMemcpyDeviceToHost);

            totalTime += (timeFFT + timeIFFT).count();

            // 释放设备内存
            cudaFree(d_N);
            cudaFree(d_M);
            cudaFree(d_fft_N);
            cudaFree(d_fft_M);
            cudaFree(d_fft_Result);
            cudaFree(d_Result);
        }

        double averageTime = totalTime / numExperiments;
        cout << "n=" << n << ", m=" << m << endl;
        cout << "平均总时间: " << averageTime * 1000 << " 毫秒" << endl;
        cout << endl;
    }

    return 0;
}
