#include <stdio.h>
#include <mpi/mpi.h>

int main(int argc, char **argv) {
    int rank=-1;
    int nbprocs=0;

    MPI_Init( &argc, &argv );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );
    MPI_Comm_size(MPI_COMM_WORLD, &nbprocs);
    printf("Hello, World!\n");
    MPI_Finalize();
    return 0;
}
