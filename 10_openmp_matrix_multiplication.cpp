#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const int n = 2;
    int a[n][n] = {{1, 2}, {3, 4}};
    int b[n][n] = {{5, 6}, {7, 8}};
    int c[n][n] = {};

    #pragma omp parallel for
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                c[i][j] += a[i][k] * b[k][j];

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) cout << c[i][j] << ' ';
        cout << '\n';
    }
}

// Compile and run:
// g++ -O2 -fopenmp 10_openmp_matrix_multiplication.cpp -o p10 && ./p10
