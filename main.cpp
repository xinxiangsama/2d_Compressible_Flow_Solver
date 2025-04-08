#include <iostream>
#include "Run.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    Run sim;
    MPI_Comm_rank(MPI_COMM_WORLD, &sim.myid);
    MPI_Comm_size(MPI_COMM_WORLD, &sim.numprocs);
    sim.initialize();
    sim.solver();
    MPI_Finalize();
    return 0;
}
