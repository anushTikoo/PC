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

// Compile and run with 4 MPI processes:
// mpic++ -O2 04_mpi_pi.cpp -o p4 && mpirun -np 4 ./p4

// WINDOWS SETUP (run once):
// 1. In Administrator PowerShell, install Ubuntu: wsl --install -d Ubuntu
// 2. Restart if prompted, launch Ubuntu from Start, then run:
//    sudo apt update && sudo apt install -y build-essential libomp-dev
//    (For MPI programs, also install: openmpi-bin libopenmpi-dev)
// 3. Run the compile/run command above inside the Ubuntu/WSL terminal, not PowerShell.
//    Your Windows C: drive is available in WSL under /mnt/c/.
// 4. Verify tools in Ubuntu: g++ --version; for MPI also run mpic++ --version and mpirun --version.
// Note: No Windows PATH edits are needed; WSL manages its own PATH.