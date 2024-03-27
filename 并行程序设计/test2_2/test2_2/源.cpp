#include<iostream>
#include<windows.h>
using namespace std;
const int n = 1000001;
int a[n];

int main()
{
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		a[i] = i;
	}
	long long head2, tail2, freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

	int sum1 = 0; int sum2 = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&head2);
	for (int i = 0; i < n - 1; i += 2)
	{
		sum1 += a[i];
		sum2 += a[i + 1];
	}
	sum = sum1 + sum2 + a[n - 1] * (n % 2);
	QueryPerformanceCounter((LARGE_INTEGER*)&tail2);
	cout << "Times2:" << (tail2 - head2) * 1000.0 / freq << "ms" << endl;
	cout << sum << endl;

}