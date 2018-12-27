#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>
int main (int argc, char **argv)
{
    int myrank, size,i;
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (myrank ==0) {
        int array[15];
        printf("Process 0 Sending data to Process 1 ... \n");
        for(i=0;i<15;i++){
            array[i] = i;
        }
        MPI_Send(&array[3],10,MPI_INT,1,666,MPI_COMM_WORLD);
    }
    else if (myrank==1){
        int x[10];
        printf("Process 1 Receiving 10 INTEGERS from Process 0 ... \n");
        MPI_Recv(x,10,MPI_INT,0,666,MPI_COMM_WORLD,&status);
        printf("Process 1 Received data from Process 0 ... \n");
        int i;
        for (i=0;i<10;i++)
            printf("x[%d]=%d\n",i,x[i]);
    }
    MPI_Finalize();
    return 0;
}