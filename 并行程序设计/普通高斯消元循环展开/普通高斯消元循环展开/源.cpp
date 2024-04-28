#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
using namespace std;
using namespace std::chrono;
const int N = 10; // �������Ĵ�С

vector<vector<double>> m(N, vector<double>(N, 0.0));

void m_reset() {
    for (int j = 0; j < N; ++j) {
        for (int i = 0; i < N; ++i) {
            m[i][j] = 0;
        }
    }
    for (int i = 0; i < N; ++i) {
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
        double akk = A[k][k];
        for (int j = k + 1; j < n; j += 4) { // ѭ��չ����ÿ�δ����ĸ�Ԫ��
            A[k][j] /= akk;
            if (j + 1 < n) {
                A[k][j + 1] /= akk;
            }
            if (j + 2 < n) {
                A[k][j + 2] /= akk;
            }
            if (j + 3 < n) {
                A[k][j + 3] /= akk;
            }
        }
        b[k] /= akk;

        for (int i = k + 1; i < n; ++i) {
            double aik = A[i][k];
            for (int j = k + 1; j < n; j += 4) { // ѭ��չ����ÿ�δ����ĸ�Ԫ��
                A[i][j] -= aik * A[k][j];
                if (j + 1 < n) {
                    A[i][j + 1] -= aik * A[k][j + 1];
                }
                if (j + 2 < n) {
                    A[i][j + 2] -= aik * A[k][j + 2];
                }
                if (j + 3 < n) {
                    A[i][j + 3] -= aik * A[k][j + 3];
                }
            }
            b[i] -= aik * b[k];
        }
    }

    // �ش�����
    vector<double> x(n);
    x[n - 1] = b[n - 1] / A[n - 1][n - 1];
    for (int i = n - 2; i >= 0; --i) {
        double sum = 0;
        for (int j = i + 1; j < n; ++j) {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }
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
    int sum = 0;
    for (int& i : time)
    {
        cout << "ִ��ʱ��: " << i << " ns" << endl;
        sum += i;
    }
    cout << "ƽ��ִ��ʱ�䣺" << sum / x << "ns" << endl;
    return 0;
}
