/**
 * Each Processor Initialized its array and then calculate the sum.
 * After that We reduced on Master Process
 */
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define ARRAY_SIZE 100
int main (int argc, char* argv[])
{
    int data[ARRAY_SIZE];
    int size, rank, i = 0;
    int sum = 0, wholeSum = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int startPoint = floor((rank*ARRAY_SIZE)/size);
    int endPoint = floor(((rank+1)*ARRAY_SIZE)/size)-1;

    for (i = startPoint; i <= endPoint; ++i) {
        data[i] = 2*i;
        sum = sum + data[i];
    }

    printf("Processor = %d, sum = %d\n", rank, sum);

    MPI_Reduce(&sum, &wholeSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        printf("Processor = %d, has Reduced whole sum = %d", rank, wholeSum);
    }

    MPI_Finalize();

    return 0;
}