#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int N = 100; // �������Ĵ�С

// �����������Ϊ vector<vector<int>>
using Matrix = vector<vector<int>>;

// ��ȡ��Ԫ�ӻ���Ԫ��
void readData(const string& filename, Matrix& data) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        int val;
        vector<int> row;
        while (iss >> val) {
            row.push_back(val);
        }
        if (row.size() == 0) continue;
        data.push_back(row);
    }
}

// �����˹��ȥ����
void gaussianElimination(Matrix& eliminators, Matrix& eliminated) {
    for (auto& row : eliminated) {
        for (auto& eliminator : eliminators) {
            if (row[0] == eliminator[0]) {
                for (size_t i = 1; i < row.size(); ++i) {
                    row[i] ^= eliminator[i];
                }
            }
        }
    }
}

// ���������ļ�
void writeData(const string& filename, Matrix& eliminated) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    for (auto& row : eliminated) {
        copy(row.begin() + 1, row.end(), ostream_iterator<int>(file, " "));
        file << endl;
    }
}

int main() {
    Matrix eliminators, eliminated;

    readData("eliminators.txt", eliminators);
    readData("eliminated.txt", eliminated);

    // �����Ԫ�Ӻͱ���Ԫ�е������Ƿ�һ��
    if (eliminators.size() != eliminated.size()) {
        cerr << "Error: Number of eliminators does not match number of eliminated rows." << endl;
        return 1;
    }

    // �����Ԫ�Ӻͱ���Ԫ�еĴ�С������
    cout << "Number of eliminators: " << eliminators.size() << endl;
    cout << "Number of eliminated rows: " << eliminated.size() << endl;

    cout << "Eliminators:" << endl;
    for (auto& row : eliminators) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    cout << "Eliminated rows:" << endl;
    for (auto& row : eliminated) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }

    vector<int> time;
    int num_trials = 10; // ִ�д���
    for (int i = 0; i < num_trials; ++i) {
        auto start = high_resolution_clock::now();
        gaussianElimination(eliminators, eliminated);
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<nanoseconds>(end - start);
        time.push_back(duration.count());
    }

    int sum = 0;
    for (int t : time) {
        sum += t;
        cout << "ִ��ʱ��: " << t << " ns" << endl;
    }
    cout << "ƽ��ִ��ʱ�䣺" << sum / num_trials << " ns" << endl;

    writeData("output.txt", eliminated);

    return 0;
}
