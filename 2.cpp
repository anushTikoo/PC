#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
using namespace std;

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    if (n <= 0) {
        cout << "Please enter a positive number of elements.\n";
        return 1;
    }

    vector<int> a(n);
    cout << "Enter " << n << " elements:\n";
    for (int i = 0; i < n; i++)
        cin >> a[i];

    int largest = a[0];

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
