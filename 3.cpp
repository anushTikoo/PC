#include <iostream>
#include <vector>
#include <omp.h>
using namespace std;

bool prime(int n) {
    if (n < 2) return false;
    for (int i = 2; i * i <= n; i++)
        if (n % i == 0) return false;
    return true;
}

int main() {
    int n;
    cout << "Enter the upper limit: ";
    cin >> n;

    if (n < 2) {
        cout << "There are no prime numbers in this range.\n";
        return 0;
    }

    vector<char> isPrime(n + 1, false);

    #pragma omp parallel for
    for (int i = 2; i <= n; i++)
        isPrime[i] = prime(i);

    cout << "Prime numbers up to " << n << ": ";
    for (int i = 2; i <= n; i++)
        if (isPrime[i]) cout << i << ' ';
    cout << '\n';
}

// Compile and run:
// g++ -O2 -fopenmp 3.cpp -o 3 && ./3

// WINDOWS SETUP (run once):
// 1. In Administrator PowerShell, install Ubuntu: wsl --install -d Ubuntu
// 2. Restart if prompted, launch Ubuntu from Start, then run:
//    sudo apt update && sudo apt install -y build-essential libomp-dev
//    (For MPI programs, also install: openmpi-bin libopenmpi-dev)
// 3. Run the compile/run command above inside the Ubuntu/WSL terminal, not PowerShell.
//    Your Windows C: drive is available in WSL under /mnt/c/.
// 4. Verify tools in Ubuntu: g++ --version; for MPI also run mpic++ --version and mpirun --version.
// Note: No Windows PATH edits are needed; WSL manages its own PATH.
