#include<iostream>
#include<windows.h>
using namespace std;
int main()
{
	long long head2, tail2, freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	const int n = 300;
	double sum2[n];
	double b[n][n];
	double a[n];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			b[i][j] = (i + j) * 1.0;
		}
		a[i] = i * 1.0;
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&head2);
	for (int i = 0; i < n; i++)
	{
		sum2[i] = 0.0;
	}
	for (int j = 0; j < n; j++)
	{
		for (int i = 0; i < n; i++)
		{
			sum2[i] += b[j][i] * a[j];
		}

	}
	QueryPerformanceCounter((LARGE_INTEGER*)&tail2);
	cout << "Times2: " << (tail2 - head2) * 1000.0 / freq << "ms" << endl;
}