#include <bits/stdc++.h>
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

// Compile and run:
// g++ -O2 -fopenmp 2.cpp -o 2 && ./2

// WINDOWS SETUP (run once):
// 1. In Administrator PowerShell, install Ubuntu: wsl --install -d Ubuntu
// 2. Restart if prompted, launch Ubuntu from Start, then run:
//    sudo apt update && sudo apt install -y build-essential libomp-dev
//    (For MPI programs, also install: openmpi-bin libopenmpi-dev)
// 3. Run the compile/run command above inside the Ubuntu/WSL terminal, not PowerShell.
//    Your Windows C: drive is available in WSL under /mnt/c/.
// 4. Verify tools in Ubuntu: g++ --version; for MPI also run mpic++ --version and mpirun --version.
// Note: No Windows PATH edits are needed; WSL manages its own PATH.