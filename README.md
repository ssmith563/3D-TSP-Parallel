Parallel 3D Traveling Salesman Problem

By Steven Smith and Matt Luckenbihl


SERIAL

Compile:   gcc -g -Wall -o tsp tsp.c -lm

Run:       ./tsp


MPI

Load:     module load mpi/openmpi-x86_64

Compile:  mpicc -g -Wall -std=c99 -o mpitsp mpitsp.c -lm

Run:      mpiexec -n <number of threads> ./mpitsp


PTHREAD

Compile: 

Run: 


OPENMP

Compile: gcc -g -Wall -o omptsp omptsp.c -lm -fopenmp

Run: ./omptsp
