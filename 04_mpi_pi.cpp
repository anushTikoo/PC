#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long n = 1000000;
    double sum = 0.0, pi;
    for (long long i = rank; i < n; i += size) {
        double x = (i + 0.5) / n;
        sum += 4.0 / (1.0 + x * x);
    }

    double local = sum / n;
    MPI_Reduce(&local, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) cout << "Pi = " << pi << '\n';

    MPI_Finalize();
}
