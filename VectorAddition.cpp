#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 100

int main (int argc, char* argv[])
{
    int *a;
    int *b;
    int *c;

    int size, rank;
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int n = ARRAY_SIZE/size;


    // Rank Zero Initialize the data
    if(rank == 0) {
        a = (int *)malloc(ARRAY_SIZE * sizeof(int));
        b = (int *)malloc(ARRAY_SIZE * sizeof(int));
        c = (int *)malloc(ARRAY_SIZE * sizeof(int));

        for (int i = 0; i < ARRAY_SIZE; ++i) {
            a[i] = i;
            b[i] = i;
        }
    }

    // Chunk Array for each process,
    int* na = (int *)malloc(n * sizeof(int));
    int* nb = (int *)malloc(n * sizeof(int));
    int* nc = (int *)malloc(n * sizeof(int));

    /**
     * NOTE: MPI_Scatter: Scatter the chunked array to all nodes including the root it-self.
     */
    // Master Process Scatter the chunk of array a and b to na and nb respectively.
    MPI_Scatter(a, n, MPI_INT, na, n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, n, MPI_INT, nb, n, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process doing vector addition on chunked array it received from Master.
    for (int j = 0; j < n; ++j) {
        nc[j] = na[j] + nb[j];
    }

    /**
     * NOTE: MPI_Gather: Gather the chunk-ed array from all nodes including the root it-self.
     */
    // Master gathered the resultant vector as chunk into the original Array c.
    MPI_Gather(nc, n, MPI_INT, c, n, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0) {
        for (int i = 0; i < ARRAY_SIZE ; ++i) {
            printf("%d\n", c[i]);
        }
        free(a); free(b); free(c);
    }

    free(na); free(nb); free(nc);

    MPI_Finalize();

    return 0;
}