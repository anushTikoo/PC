#include <iostream>
#include <algorithm>
#include <vector>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    vector<int> all = {9, 3, 7, 1, 8, 2, 6, 4};
    int n = all.size();
    if (n % size != 0) {
        if (rank == 0) cerr << "Array size must be divisible by process count.\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int m = n / size;
    vector<int> local(m), other(m);
    MPI_Scatter(all.data(), m, MPI_INT, local.data(), m, MPI_INT, 0, MPI_COMM_WORLD);
    sort(local.begin(), local.end());

    for (int phase = 0; phase < size; phase++) {
        int partner = (phase % 2 == 0) ? ((rank % 2 == 0) ? rank + 1 : rank - 1)
                                       : ((rank % 2 == 0) ? rank - 1 : rank + 1);
        if (partner < 0 || partner >= size) continue;

        MPI_Sendrecv(local.data(), m, MPI_INT, partner, 0,
                     other.data(), m, MPI_INT, partner, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        vector<int> merged(2 * m);
        merge(local.begin(), local.end(), other.begin(), other.end(), merged.begin());
        if (rank < partner)
            copy(merged.begin(), merged.begin() + m, local.begin());
        else
            copy(merged.end() - m, merged.end(), local.begin());
    }

    MPI_Gather(local.data(), m, MPI_INT, all.data(), m, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        cout << "Sorted: ";
        for (int x : all) cout << x << ' ';
        cout << '\n';
    }

    MPI_Finalize();
}
