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
	long long head4, tail4, freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

	
	QueryPerformanceCounter((LARGE_INTEGER*)&head4);
	for (int m = n; m > 1; m /= 2)
	{
		for (int i = 0; i < m / 2; i++)
		{
			a[i] = a[i * 2] + a[i * 2 + 1];
		}
		a[0] += a[m - 1] * (m % 2);
	}
	sum = a[0];
	QueryPerformanceCounter((LARGE_INTEGER*)&tail4);
	cout << "Times4:" << (tail4 - head4) * 1000.0 / freq << "ms" << endl;
	cout << sum << endl;
}