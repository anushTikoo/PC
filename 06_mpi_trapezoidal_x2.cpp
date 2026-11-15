#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const double a = 0.0, b = 1.0;
    const int n = 1000;
    double h = (b - a) / n, local = 0.0, total;

    for (int i = rank; i < n; i += size) {
        double x1 = a + i * h, x2 = x1 + h;
        local += (x2 * x2 + x1 * x1) * h / 2.0;
    }

    MPI_Reduce(&local, &total, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) cout << "Integral = " << total << '\n';

    MPI_Finalize();
}

// Compile and run with 4 MPI processes:
// mpic++ -O2 06_mpi_trapezoidal_x2.cpp -o p6 && mpirun -np 4 ./p6
