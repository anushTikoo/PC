#include <iostream>
#include <algorithm>
#include <omp.h>
using namespace std;

int main() {
    int a[] = {12, 45, 7, 89, 23, 56, 91, 3};
    int n = sizeof(a) / sizeof(a[0]), largest = a[0];

    #pragma omp parallel for reduction(max:largest)
    for (int i = 0; i < n; i++)
        largest = max(largest, a[i]);

    cout << "Largest = " << largest << '\n';
}
