/*
 ============================================================================
 Name        : Esercitazione1MNI.c
 Author      : Giovanni Leo
 Number      : 0522500538
 Description : Implementazione della seconda strategia di somma di N numeri
 	 	 	   vista in classe con i double.
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

int main (int argc, char**argv)
{
	int rank;      //processors rank
	int numP;	   //processors number
	MPI_Status status;

	//MPI Inizialization
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numP);


	int multiply = atof(argv[1]);
	int N = 1000000*multiply;

	double*  array = (double *)malloc(sizeof(double)*N);
	int i;

	for (i = 0; i < N; i++) {
		array[i] = 1.1;
	}

	int chunck = N/numP;
	int remainder = N % numP;
	int start,stop;

	//Start work partitioning
	if (rank < remainder) {
		// The first 'remainder' ranks get 'count + 1' tasks each
		start = rank * (chunck + 1);
		stop = start + chunck;

	} else {
		// The remaining 'size - remainder' ranks get 'count' task each
		start = rank * chunck + remainder;
		stop = start + (chunck - 1);

	}
	//End work partitioning
	double startTime = MPI_Wtime();
	double partialSum = 0,recivedPartialSum = 0;

	for(i = start ; i <= stop; i++)
	{
		partialSum += array[i];
	}

	int itereation = 0, p = numP;

	//logarithm operation using bitwise
	while(p!=1)
	{
		p = p >> 1;
		itereation++;
	}


	int dist,rankToComunicate,temp;

	for(i = 0; i < itereation;i++)
	{

		if(i == 0)
		{
			dist = 1;
			temp = 2;
		}
		else
		{
			temp =(2 << i); //is equivalent to 2^i,in the pseudocode is equivalent to 2^(k+1)
			dist = temp >>1; //is equivalent to 2^i/2, in the pseudocode is equivalent to 2^(k)
		}
		//Computed dist doing first a multiplication and then a division is equivalent to (2^i)/2

		if((rank%temp) == 0)
		{
			rankToComunicate = rank + dist;
			MPI_Recv(&recivedPartialSum,1,MPI_DOUBLE,rankToComunicate,0,MPI_COMM_WORLD,&status);
			partialSum+=recivedPartialSum;

		}
		else if((rank%temp) == dist)
		{
			rankToComunicate = rank - dist;
			MPI_Send(&partialSum,1,MPI_DOUBLE,rankToComunicate,0,MPI_COMM_WORLD);
		}

	}
	double endTime = MPI_Wtime();
	double elapsed = endTime-startTime;
	double max;
	MPI_Reduce(&elapsed,&max,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

	if (rank == 0)
	{

		printf("Elapsed time %lf\n",max);
		printf("===================================\n");
		printf("Partial Sum %lf\n",partialSum);

	}
	//MPI End
	MPI_Finalize();
	return 0;
}
