/*
 ============================================================================
 Name        : Esercitazione2MNIParte2
 Author      : Giovanni Leo
 Number      : 0522500538
 Description : prodotto mat vet sequenziale double
 ============================================================================
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "mpi.h"

double matrixGetElement(double* A, int ROWS, int COLS, int r, int c);
void matVetProduct(double y[], double *a, int ROWS, int COLS, double x[]);

int main (int argc, char* argv[])
{
	int rank;      //processors rank
	int numP;	   //processors number
	MPI_Status status;

	//MPI Inizialization
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numP);

	int cols,rows;
	cols = atoi(argv[1])*1000;
	rows = cols;


	int N = rows*cols;
	double* y = (double*)malloc(sizeof(double)*(rows));
	double* A = (double *)malloc(sizeof(double)*N);
	double* x = (double *)malloc(sizeof(double)*cols);


	int i ,j;


	for (i=0;i<rows;i++)
	{
		x[i]= i;
		for(j=0;j<cols;j++)
		{
			if (j==0)
				A[(i * cols) + j] = (1.0/(i+1))-1;
			else
				A[(i * cols) + j] = (1.0/(i+1))-(pow(1.0/2.0,j));

		}

	}





	if(rows < 10 && cols < 10)
	{
		for(i = 0;i< rows;i++)
		{
			for(j=0;j<cols;j++)
			{
				printf("%d ",matrixGetElement(A, rows, cols, j, i));
			}
			printf("\n");

		}
	}



	double startTime = MPI_Wtime();

	matVetProduct(y, A, rows,  cols, x);

	double endTime = MPI_Wtime();
	double elapsed = endTime-startTime;


	if(rank == 0)
	{
		int i;
		if(rows < 10 && cols < 10)
		{
			for(i = 0; i < (rows); i++)
			{
				printf("%lf \n",y[i]);

			}
		}

		printf("Elapsed %lf \n",elapsed);

	}

	MPI_Finalize();
	return 0;

}

/**
 * Funzione per ottenere un elemento della matrice.
 * A     puntatore alla matrice
 * ROWS  numero di righe della matrice
 * COLS  numero di colonne della matrice
 * r     riga dell'elemento da prelevare
 * c     colonna dell'elemento da prelevare
 */
double matrixGetElement(double* A, int ROWS, int COLS, int r, int c)
{
	return A[ (c * COLS) + r ];
}

void matVetProduct(double y[], double *a, int ROWS, int COLS, double x[])
{
	int i, j;

	for(i=0;i<ROWS;i++)
	{
		y[i]=0;
		for(j=0;j<COLS;j++)
		{
			y[i] += a[i*COLS+j] * x[j];

		}

	}
}

