#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

bool isPrime(int num)
{
    if(num == 2)
        return 1;
    else if (num%2==0)
        return 0;
    for (int i=3;i<sqrt(num);i+=2)
        if (num%i==0)
            return 0;
    return 1;
}

int n;
int world_size; //number of processes involved in our calculations

int *curPrimes;//the primes that we will calculate in THIS process
int k = 0;

int *recvPrimes;//temperary variable for primes the main process will recieve from secondary ones


int main(int argc, char** argv) {

    int i;
    //check if we have the right number of arguments
    /*if (argc!=2)
    {
        printf("invalid number of arguments");
        return 0;
    }*/
    //calculate n as per requrenment
    n = 100;
    //we cant have more than n/2 primes becuase even numbers for sure cant be prime

    //init mpi
    MPI_Init(&argc, &argv);

    //find out how many processes are there
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    //find out which process are we in
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int start = n*world_rank / world_size;
    int end = n*(world_rank + 1) / world_size;
    int chunk = n/world_size;

    curPrimes = (int *) malloc(chunk * sizeof(int));
    //calculate prime numbers each process will calculate its own portion
    for (i = start;i < end; i++)
        if (isPrime(i))
            curPrimes[k++]=i;
    //if we are not in the main process(process with rank 0) send our prime numbers to main process
    if (world_rank!=0)
    {
        MPI_Send(&k, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(curPrimes, k, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    int recvSize;

    if (world_rank==0)
    {
        //this runs only on main process
        //add the primes calculated here to our large prime vector
        for (i=0;i<k;i++)
        {
            printf("%d\n", curPrimes[i]);
        }
        for (i=1;i<world_size;i++)
        {
            MPI_Recv(&recvSize, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            recvPrimes = (int *) malloc(recvSize * sizeof(int));
            MPI_Recv(recvPrimes, recvSize, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int j=0;j<recvSize;j++)
            {
                printf("%d\n", recvPrimes[j]);
            }
        }
    }



    MPI_Finalize();
    return 0;
}