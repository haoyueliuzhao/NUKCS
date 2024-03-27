#include<iostream>
#include<windows.h>
using namespace std;
int main()
{
	long long head1, tail1,  freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	const int n = 300;
	double sum1[n];
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
	QueryPerformanceCounter((LARGE_INTEGER*)&head1);
	for (int i = 0; i < n; i++)
	{

		sum1[i] = 0.0;
		for (int j = 0; j < n; j++)
		{
			sum1[i] += b[j][i] * a[j];
		}


	}
	QueryPerformanceCounter((LARGE_INTEGER*)&tail1);
	cout << "Times1: " << (tail1 - head1) * 1000.0 / freq << "ms" << endl;
}