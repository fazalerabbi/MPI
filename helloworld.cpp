#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    int size, rank, length;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Get_processor_name(processor_name, &length);

    printf("%s = %d/%d\n", processor_name, rank, size);

    MPI_Finalize();
    return 0;
}