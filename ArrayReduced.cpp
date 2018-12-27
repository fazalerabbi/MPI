/**
 * Array Reduced work as:
 *  recvbuffer[i] = sendbuffer(0th Rank)[i] op sendbuffer(1th Rank)[i] op sendbuffer(2nd Rank)[i] op sendbuffer(nth Rank)[i]
 */
#include<stdio.h>
#include<mpi.h>

#define ARRAY_SIZE 100
int main(int argc , char **argv)
{
    int size, rank,i;
    int sendbuffer[ARRAY_SIZE];
    int recvbuffer[ARRAY_SIZE];
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    for(i = 0;i <= ARRAY_SIZE;i++){
        sendbuffer[i] = 2 * i;
    }

    MPI_Reduce(&sendbuffer,&recvbuffer,ARRAY_SIZE,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
    if(rank==0){
        for(i=0;i<ARRAY_SIZE;i++) {
            printf("recvbuffer[%d] =%d\n",i,recvbuffer[i]);
        }
    }

    MPI_Finalize();
}