#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;
const int N = 260; // �������Ĵ�С

vector<vector<double>> m(N, vector<double>(N, 0.0));

void m_reset() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            m[i][j] = 0;
        }
        m[i][i] = 1.0;
        for (int j = i + 1; j < N; ++j) {
            m[i][j] = static_cast<double>(rand()) / RAND_MAX; // �������һ��0��1֮��ĸ�����
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

    // ��Ԫ����
    for (int k = 0; k < n; ++k) {
        for (int i = k + 1; i < n; ++i) {
            double factor = A[i][k] / A[k][k];
            for (int j = k + 1; j < n; ++j) {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
        }
    }

    // �ش�����
    vector<double> x(n);
    x[n - 1] = b[n - 1] / A[n - 1][n - 1];
    for (int i = n - 2; i >= 0; --i) {
        double sum = b[i];
        for (int j = i + 1; j < n; ++j) {
            sum -= A[i][j] * x[j];
        }
        x[i] = sum / A[i][i];
    }

    // ��ӡ������
    /*cout << "������ x:" << endl;
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
        auto start = high_resolution_clock::now(); // ��ȡ��ǰʱ���
        Gaussian_Elimination(m, b);
        auto end = high_resolution_clock::now(); // ��ȡ��ǰʱ���
        auto duration = duration_cast<nanoseconds>(end - start); // ����ʱ���
        time.push_back(duration.count());
        //cout << "ִ��ʱ��: " << duration.count() << " ns" << endl;
        
    }
    int sum=0;
    for (int& i : time)
    {
        cout << "ִ��ʱ��: " << i << " ns" << endl;
        sum += i;
    }
    cout << "ƽ��ִ��ʱ�䣺" << sum / x << "ns" << endl;
    return 0;
}
