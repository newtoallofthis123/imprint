#include <iostream>

using namespace std;

int main() {
    int N;
    while (cin >> N && N != 0) {
        long long result = 0;
        for (int i = 1; i <= N; ++i) {
            result += i * i;
        }
        cout << result << endl;
    }
    return 0;
}
