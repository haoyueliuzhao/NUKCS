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
	long long head1, tail1,freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);

	QueryPerformanceCounter((LARGE_INTEGER*)&head1);
	for (int i = 0; i < n; i++)
	{
		sum += a[i];
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&tail1);
	cout << "Times1:" << (tail1 - head1) * 1000.0 / freq << "ms" << endl;
	cout << sum << endl;

	
}