/**
 * Simulating that We are getting data from some other source like file.
 * That data is already initialized.
 * We need to assign the data to each process. that wil do sum fo array on their part.
 * After that we will reduce the sum.
 */
#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define ARRAY_SIZE 100
int data[ARRAY_SIZE];

int update(int offset, int chunk, int id);

int main (int argc, char* argv[])
{

    int size, rank, i = 0;
    int sum = 0, wholeSum = 0;
    int tag1 = 1, tag2 = 2;
    MPI_Status status;
    int value = 1;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int chunkSize = ARRAY_SIZE/size;
    int offset = 0;

    if(rank == 0) {
        for (i = 0; i < ARRAY_SIZE; ++i) {
            data[i] = 2*i;
        }
        offset = chunkSize;

        for (i = 1; i < size; i++) {
            MPI_Send(&offset, 1, MPI_INT, i, tag1, MPI_COMM_WORLD);
            MPI_Send(&data[offset], chunkSize, MPI_INT, i, tag2, MPI_COMM_WORLD);

            printf("Sent %d elements to task %d offset= %d, data = %d\n", chunkSize, i, offset, data[offset]);
            offset = offset + chunkSize;
        }

        /* Master does its part of the work */
        offset = 0;
        sum = update(offset, chunkSize, rank);

        MPI_Reduce(&sum, &wholeSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        printf("*** Final sum= %d ***\n",wholeSum);
    }
    if (rank > 0) {

        /* Receive my portion of array from the master task */
        int source = 0;
        MPI_Recv(&offset, 1, MPI_INT, source, tag1, MPI_COMM_WORLD, &status);
        MPI_Recv(&data[offset], chunkSize, MPI_INT, source, tag2, MPI_COMM_WORLD, &status);

        printf("Receive => Rank = %d, value = %d\n", rank, data[offset]);
        sum = update(offset, chunkSize, rank);
        printf("Send => Rank = %d, value = %d\n", rank, data[offset]);
        /* Send my results back to the master task */

        MPI_Reduce(&sum, &wholeSum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    } /* end of non-master */

    MPI_Finalize();

    return 0;
}

int update(int offset, int chunk, int id)
{
    int sum = 0;
    for (int i = offset; i < offset + chunk; ++i) {
        sum = sum + data[i];
    }

    return sum;
}

