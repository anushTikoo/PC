# Parallel Programming Lab Programs (C++)

Ten standalone, concise C++ implementations based on the supplied lab list.

## Requirements
- OpenMP: g++ with OpenMP support.
- MPI: Open MPI or another MPI implementation providing mpic++ and mpirun.

## Compile and run

OpenMP:
```bash
g++ -O2 -fopenmp 01_openmp_pi.cpp -o p1 && ./p1
g++ -O2 -fopenmp 02_openmp_largest.cpp -o p2 && ./p2
g++ -O2 -fopenmp 03_openmp_primes.cpp -o p3 && ./p3 100
g++ -O2 -fopenmp 09_openmp_producer_consumer.cpp -o p9 && ./p9
g++ -O2 -fopenmp 10_openmp_matrix_multiplication.cpp -o p10 && ./p10
```

MPI:
```bash
mpic++ -O2 04_mpi_pi.cpp -o p4 && mpirun -np 4 ./p4
mpic++ -O2 05_mpi_ring.cpp -o p5 && mpirun -np 4 ./p5
mpic++ -O2 06_mpi_trapezoidal_x2.cpp -o p6 && mpirun -np 4 ./p6
mpic++ -O2 07_mpi_odd_even_sort.cpp -o p7 && mpirun -np 4 ./p7
mpic++ -O2 08_mpi_hyperquicksort.cpp -o p8 && mpirun -np 4 ./p8
```

Program 7 requires an array size divisible by the process count. Program 8 requires a power-of-two process count and a divisible array size. The examples use fixed, small arrays and values to keep the code easy to revise.
