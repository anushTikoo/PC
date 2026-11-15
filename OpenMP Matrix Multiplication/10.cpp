#include <bits/stdc++.h>
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
// g++ -O2 -fopenmp 10.cpp -o 10 && ./10

// WINDOWS SETUP (run once):
// 1. In Administrator PowerShell, install Ubuntu: wsl --install -d Ubuntu
// 2. Restart if prompted, launch Ubuntu from Start, then run:
//    sudo apt update && sudo apt install -y build-essential libomp-dev
//    (For MPI programs, also install: openmpi-bin libopenmpi-dev)
// 3. Run the compile/run command above inside the Ubuntu/WSL terminal, not PowerShell.
//    Your Windows C: drive is available in WSL under /mnt/c/.
// 4. Verify tools in Ubuntu: g++ --version; for MPI also run mpic++ --version and mpirun --version.
// Note: No Windows PATH edits are needed; WSL manages its own PATH.