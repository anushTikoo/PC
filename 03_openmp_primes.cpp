#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>
using namespace std;

bool prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

int main(int argc, char *argv[]) {
    int n = (argc > 1) ? atoi(argv[1]) : 50;
    if (n < 2) {
        cout << '\n';
        return 0;
    }

    vector<char> isPrime(n + 1, false);
    #pragma omp parallel for
    for (int i = 2; i <= n; i++)
        isPrime[i] = prime(i);

    for (int i = 2; i <= n; i++)
        if (isPrime[i]) cout << i << ' ';
    cout << '\n';
}