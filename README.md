Parallel 3D Traveling Salesman Problem

By Steven Smith and Matt Luckenbihl


SERIAL - (Using omp get wtime so need -fopenmp)

Compile:   gcc -g -Wall -o tsp tsp.c -lm -fopenmp

Run:       ./tsp


MPI

Load:     module load mpi/openmpi-x86_64

Compile:  mpicc -g -Wall -std=c99 -o mpitsp mpitsp.c -lm

Run:      mpiexec -n <number of threads> ./mpitsp


PTHREAD

Compile:  gcc -g -Wall -o ptsp ptsp.c -lpthread -lm

Run:      ./ptsp <num threads>  


OPENMP

Compile: gcc -g -Wall -o omptsp omptsp.c -lm -fopenmp

Run:      ./omptsp

Once started, enter range for x, y, & z
Then, enter a value for the number of nodes to visit. 
