#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const long long n = 1000000;
    double sum = 0.0;

    #pragma omp parallel for reduction(+:sum)
    for (long long i = 0; i < n; i++) {
        double x = (i + 0.5) / n;
        sum += 4.0 / (1.0 + x * x);
    }

    cout << "Pi = " << sum / n << '\n';
}

// Compile and run:
// g++ -O2 -fopenmp 01_openmp_pi.cpp -o p1 && ./p1
