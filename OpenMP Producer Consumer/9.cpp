#include <bits/stdc++.h>
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
                cout << "Consumed: " << value << '\n';
            }
        }
    }
}

// Compile and run:
// g++ -O2 -fopenmp 9.cpp -o 9 && ./9

// WINDOWS SETUP (run once):
// 1. In Administrator PowerShell, install Ubuntu: wsl --install -d Ubuntu
// 2. Restart if prompted, launch Ubuntu from Start, then run:
//    sudo apt update && sudo apt install -y build-essential libomp-dev
//    (For MPI programs, also install: openmpi-bin libopenmpi-dev)
// 3. Run the compile/run command above inside the Ubuntu/WSL terminal, not PowerShell.
//    Your Windows C: drive is available in WSL under /mnt/c/.
// 4. Verify tools in Ubuntu: g++ --version; for MPI also run mpic++ --version and mpirun --version.
// Note: No Windows PATH edits are needed; WSL manages its own PATH.