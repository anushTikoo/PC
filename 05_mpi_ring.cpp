#include <iostream>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size, value;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;

    if (rank == 0) {
        value = 1;
        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        cout << "Ring completed. Value = " << value << '\n';
    } else {
        MPI_Recv(&value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        value++;
        MPI_Send(&value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}

// Compile and run with 4 MPI processes:
// mpic++ -O2 05_mpi_ring.cpp -o p5 && mpirun -np 4 ./p5
