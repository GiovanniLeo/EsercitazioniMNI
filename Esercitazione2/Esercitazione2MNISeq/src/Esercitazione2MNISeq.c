/*
 ============================================================================
 Name        : Esercitazione2MNIParte2
 Author      : Giovanni Leo
 Number      : 0522500538
 Description : Esercitazione2MNIParte2, l'idea è quella di fare la trapossta
 della matrice e dividerla nello stesso modo della seconda strategia e poi
 una volta inviata si rifà la trasposta al fine di calcolare il prodotto
 matrice vettore. In questo caso vengolo trattate le matrici rettangolari
 le quali devono avaere un numero di colonne pari.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int matrixGetElement(int* A, int ROWS, int COLS, int r, int c);
void matVetProduct(int y[], int *a, int ROWS, int COLS, int x[]);

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
	cols = atoi(argv[1]);
	rows = atoi(argv[2]);

	int N = rows*cols;
	int* y = (int*)malloc(sizeof(int)*(rows));
	int* A = (int *)malloc(sizeof(int)*N);
	int* x = (int *)malloc(sizeof(int)*cols);


	int i ,j;

	for(i = 0; i < (cols*rows); i++)
	{
		A[i] = i + 1;
	}

	for(i = 0; i < cols; i++)
	{
		x[i] = 1;
	}

	for(i = 0;i< rows;i++)
	{
		for(j=0;j<cols;j++)
		{
			printf("%d ",matrixGetElement(A, rows, cols, j, i));
		}
		printf("\n");

	}

	double startTime = MPI_Wtime();

	matVetProduct(y, A, rows,  cols, x);

	double endTime = MPI_Wtime();
	double elapsed = endTime-startTime;


	if(rank == 0)
	{
		int i;

		for(i = 0; i < (rows); i++)
		{
			printf("%d \n",y[i]);

		}

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
int matrixGetElement(int* A, int ROWS, int COLS, int r, int c)
{
	return A[ (c * COLS) + r ];
}

void matVetProduct(int y[], int *a, int ROWS, int COLS, int x[])
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

