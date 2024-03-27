#include<iostream>
#include<windows.h>
using namespace std;
const int n = 1000001;
int a[n];
void recursion(int n)
{
	if (n == 1)
	{
		return;
	}
	else
	{
		for (int i = 0; i < n / 2; i++)
		{
			a[i] += a[n - i - 1];
		}
		a[0] += a[n - 1] * (n % 2);
		n = n / 2;
		recursion(n);
	}
}

int main()
{
	int sum = 0;
	for (int i = 0; i < n; i++)
	{
		a[i] = i;
	}
	long long head3, tail3,  freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

	QueryPerformanceCounter((LARGE_INTEGER*)&head3);
	recursion(n);
	sum = a[0];
	QueryPerformanceCounter((LARGE_INTEGER*)&tail3);
	cout << "Times3:" << (tail3 - head3) * 1000.0 / freq << "ms" << endl;
	cout << sum << endl;
	
}