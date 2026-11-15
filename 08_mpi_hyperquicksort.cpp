#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <mpi.h>
using namespace std;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int levels = 0;
    while ((1 << levels) < size) levels++;
    if ((1 << levels) != size) {
        if (rank == 0) cerr << "Use a power-of-two process count.\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    vector<int> data = {19, 4, 15, 2, 17, 8, 11, 1, 20, 6, 13, 3, 18, 9, 12, 5};
    int n = data.size();
    if (n % size != 0) {
        if (rank == 0) cerr << "Array size must be divisible by process count.\n";
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int m = n / size;
    vector<int> local(m);
    MPI_Scatter(data.data(), m, MPI_INT, local.data(), m, MPI_INT, 0, MPI_COMM_WORLD);
    sort(local.begin(), local.end());

    for (int d = levels - 1; d >= 0; d--) {
        int color = rank >> (d + 1);
        MPI_Comm group;
        MPI_Comm_split(MPI_COMM_WORLD, color, rank, &group);

        int gsize, grank;
        MPI_Comm_size(group, &gsize);
        MPI_Comm_rank(group, &grank);

        int median = local.empty() ? (rank & (1 << d) ? 2147483647 : -2147483647) : local[local.size() / 2];
        vector<int> medians(grank == 0 ? gsize : 0);
        MPI_Gather(&median, 1, MPI_INT, medians.data(), 1, MPI_INT, 0, group);

        int pivot = 0;
        if (grank == 0) {
            sort(medians.begin(), medians.end());
            pivot = medians[gsize / 2];
        }
        MPI_Bcast(&pivot, 1, MPI_INT, 0, group);
        MPI_Comm_free(&group);

        vector<int> low, high;
        for (int x : local)
            (x <= pivot ? low : high).push_back(x);

        int partner = rank ^ (1 << d);
        int sendCount = (rank & (1 << d)) ? (int)low.size() : (int)high.size();
        int recvCount = 0;
        MPI_Sendrecv(&sendCount, 1, MPI_INT, partner, 0,
                     &recvCount, 1, MPI_INT, partner, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        vector<int> send(sendCount), recv(recvCount);
        if (rank & (1 << d)) send = low;
        else send = high;

        MPI_Sendrecv(send.data(), sendCount, MPI_INT, partner, 1,
                     recv.data(), recvCount, MPI_INT, partner, 1,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        vector<int> combined;
        if (rank & (1 << d)) {
            combined = high;
            combined.insert(combined.end(), recv.begin(), recv.end());
        } else {
            combined = low;
            combined.insert(combined.end(), recv.begin(), recv.end());
        }
        sort(combined.begin(), combined.end());
        if ((int)combined.size() != 2 * m) {
            cerr << "Unexpected partition size on rank " << rank << '\n';
            MPI_Abort(MPI_COMM_WORLD, 2);
        }
        if (rank & (1 << d))
            local.assign(combined.end() - m, combined.end());
        else
            local.assign(combined.begin(), combined.begin() + m);
    }

    MPI_Gather(local.data(), m, MPI_INT, data.data(), m, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        cout << "Sorted: ";
        for (int x : data) cout << x << ' ';
        cout << '\n';
    }
    MPI_Finalize();
}
