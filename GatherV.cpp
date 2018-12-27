#include <mpi.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
    int rank;
    int size;
    int i;
    int data[10];
    int rcv_counts[10] = {1,1,2,2,1,1,1,1};
    int displ[10] = {0,1,2,4,6,7,8,9};
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size != 10){
        if (rank == 0){
            printf("Please run with 10 processes\n");
            fflush(stdout);
        }
        MPI_Finalize();
        return 0;
    }
    for (i=0; i<rank; i++){
        data[i] = rank;
    }
    MPI_Gatherv(data, rank, MPI_INT, data, rcv_counts, displ, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0){
        for (i=0; i<10; i++){
            printf("[%d]", data[i]);
        }
        printf("\n");
        fflush(stdout);
    }
    MPI_Finalize();
    return 0;
}