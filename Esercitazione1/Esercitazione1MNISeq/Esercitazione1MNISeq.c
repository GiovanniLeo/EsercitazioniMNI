#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"


int main (int argc , char* argv[])
{

	int rank;      //processors rank
	int numP;	   //processors number
	MPI_Status status;

	//MPI Inizialization
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numP);

	double start;
	double end;
	int lenghtMult = atoi(argv[1]);
	int arrayLenght = 1000000 * lenghtMult;
	int* array = (int *)malloc(sizeof(int)*arrayLenght);


	int i = 0;
	int sum = 0;

	for(i = 0; i < arrayLenght; i++)
	{
		array[i] = rand()%20+1;
	}

	start = MPI_Wtime();
	//clock_t begin = clock();

	for(i = 0; i < arrayLenght; i++)
	{
		sum+=array[i];
	}
	//clock_t end2 = clock();

	//printf("The sum is %ld\n",sum);

	end = MPI_Wtime();

	double finalTime = (double)(end- start);
	printf("Elapsed time:%lf\n",finalTime);
	//printf("Elapsed time:%lf\n",(double)(end2-begin)/CLOCKS_PER_SEC);
	printf("sum:%d\n",sum);

	MPI_Finalize();
	return 0;

}
