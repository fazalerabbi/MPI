#include<stdio.h>
#include<mpi.h>
#include<math.h>
#define ARRAYSIZE 100
int main(int argc , char **argv)
{
    int size, rank,i=0,localsum=0,globalsum=0;
    int data[ARRAYSIZE];
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int start = floor(rank*ARRAYSIZE/size);
    int end = floor((rank+1)*ARRAYSIZE/size)-1;
    for(i=start;i<=end;i++)	{
        /*initialize the array*/
        data[i] = i*2;
        /*alculate localsum*/
        localsum = localsum + data[i];
    }
    MPI_Allreduce(&localsum,&globalsum,1,MPI_INT,MPI_SUM,MPI_COMM_WORLD);
    printf("I am Process %d The global sum =%d\n",rank,globalsum);
    MPI_Finalize();
}