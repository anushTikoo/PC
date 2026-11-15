#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    const int n = 10;
    int buffer = 0, ready = 0;

    #pragma omp parallel num_threads(2) shared(buffer, ready)
    {
        if (omp_get_thread_num() == 0) {
            for (int i = 1; i <= n; i++) {
                int done = 0;
                while (!done) {
                    #pragma omp critical
                    if (!ready) {
                        buffer = i;
                        ready = 1;
                        done = 1;
                    }
                }
            }
        } else {
            for (int i = 1; i <= n; i++) {
                int done = 0, value = 0;
                while (!done) {
                    #pragma omp critical
                    if (ready) {
                        value = buffer;
                        ready = 0;
                        done = 1;
                    }
                }
                cout << "Consumed: " << value << '\\n';
            }
        }
    }
}

// Compile and run:
// g++ -O2 -fopenmp 09_openmp_producer_consumer.cpp -o p9 && ./p9
