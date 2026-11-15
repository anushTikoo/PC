#include <iostream>
#include <vector>
#include <algorithm>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int levels = 0;
    while ((1 << levels) < size) levels++;
    if ((1 << levels) != size) MPI_Abort(MPI_COMM_WORLD, 1);

    vector<int> data = {19, 4, 15, 2, 17, 8, 11, 1, 20, 6, 13, 3, 18, 9, 12, 5};
    int n = data.size();
    if (n % size) MPI_Abort(MPI_COMM_WORLD, 1);

    int m = n / size;
    vector<int> local(m);
    MPI_Scatter(data.data(), m, MPI_INT, local.data(), m, MPI_INT, 0, MPI_COMM_WORLD);
    sort(local.begin(), local.end());

    for (int d = levels - 1; d >= 0; d--) {
        MPI_Comm group;
        MPI_Comm_split(MPI_COMM_WORLD, rank >> (d + 1), rank, &group);

        int grank, gsize;
        MPI_Comm_rank(group, &grank);
        MPI_Comm_size(group, &gsize);

        int mid = local.empty() ? ((rank & (1 << d)) ? 2147483647 : -2147483647)
                                : local[local.size() / 2];
        vector<int> mids(grank == 0 ? gsize : 0);
        MPI_Gather(&mid, 1, MPI_INT, mids.data(), 1, MPI_INT, 0, group);

        int pivot = 0;
        if (grank == 0) {
            sort(mids.begin(), mids.end());
            pivot = mids[gsize / 2];
        }
        MPI_Bcast(&pivot, 1, MPI_INT, 0, group);
        MPI_Comm_free(&group);

        vector<int> low, high;
        for (int x : local) (x <= pivot ? low : high).push_back(x);

        int partner = rank ^ (1 << d);
        vector<int> send = (rank & (1 << d)) ? low : high;
        int sendCount = send.size(), recvCount;
        MPI_Sendrecv(&sendCount, 1, MPI_INT, partner, 0,
                     &recvCount, 1, MPI_INT, partner, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        vector<int> recv(recvCount);
        MPI_Sendrecv(send.data(), sendCount, MPI_INT, partner, 1,
                     recv.data(), recvCount, MPI_INT, partner, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        local = (rank & (1 << d)) ? high : low;
        local.insert(local.end(), recv.begin(), recv.end());
        sort(local.begin(), local.end());
    }

    int count = local.size();
    vector<int> counts(rank == 0 ? size : 0), displs(rank == 0 ? size : 0);
    MPI_Gather(&count, 1, MPI_INT, counts.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
        for (int i = 1; i < size; i++) displs[i] = displs[i - 1] + counts[i - 1];

    vector<int> result(rank == 0 ? n : 0);
    MPI_Gatherv(local.data(), count, MPI_INT, result.data(), counts.data(),
                displs.data(), MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Sorted: ";
        for (int x : result) cout << x << ' ';
        cout << '\n';
    }
    MPI_Finalize();
}

// Compile and run with 4 MPI processes (use a power-of-two process count):
// mpic++ -O2 08_mpi_hyperquicksort.cpp -o p8 && mpirun -np 4 ./p8

// WINDOWS SETUP (run once):
// 1. In Administrator PowerShell, install Ubuntu: wsl --install -d Ubuntu
// 2. Restart if prompted, launch Ubuntu from Start, then run:
//    sudo apt update && sudo apt install -y build-essential libomp-dev
//    (For MPI programs, also install: openmpi-bin libopenmpi-dev)
// 3. Run the compile/run command above inside the Ubuntu/WSL terminal, not PowerShell.
//    Your Windows C: drive is available in WSL under /mnt/c/.
// 4. Verify tools in Ubuntu: g++ --version; for MPI also run mpic++ --version and mpirun --version.
// Note: No Windows PATH edits are needed; WSL manages its own PATH.