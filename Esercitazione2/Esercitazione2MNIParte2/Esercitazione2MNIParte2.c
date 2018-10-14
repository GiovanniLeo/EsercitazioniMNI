/*
 ============================================================================
 Name        : Esercitazione2MNIParte2
 Author      : Giovanni Leo
 Number      : 0522500538
 Description : Esercitazione2MNIParte2, l'idea è quella di fare la trapossta
 della matrice e dividerla nello stesso modo della seconda strategia e poi
 una volta inviata si rifà la trasposta al fine di calcolare il prodotto
 matrice vettore.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

void traspose(int* array, int cols, int rows);
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
	cols = atoi(argv[1]) * 1000;
	//rows = atoi(argv[2]);
	rows = cols;
	int N = rows*cols;
	int chunck = N/numP;
	int xChunck = rows/numP;
	int startA,startX;
	int* localA = (int*)malloc(sizeof(int)*chunck);
	int* localX = (int*)malloc(sizeof(int)*xChunck);
	int* localY = (int*)malloc(sizeof(int)*(rows));
	int* y = (int*)malloc(sizeof(int)*(rows));
	int* A;
	int* x;


	if(rank == 0)
	{

		A = (int *)malloc(sizeof(int)*N);
		x = (int *)malloc(sizeof(int)*rows);
		int i,j;
		for(i = 0; i < (cols*rows); i++)
		{
			A[i] = i + 1;
		}

		for(i = 0; i < rows; i++)
		{
			x[i] = 1;
		}

		if(rows < 10 && cols < 10)
		{

			for(i = 0;i< cols;i++)
			{
				for(j=0;j<rows;j++)
				{
					printf("%d ",matrixGetElement(A, rows, cols, j, i));
				}
				printf("\n");

			}
		}

		traspose(A, rows, cols);
	}


	MPI_Scatter(&x[0], xChunck, MPI_INT,&localX[0], xChunck, MPI_INT,0, MPI_COMM_WORLD);
	MPI_Scatter(&A[0], chunck, MPI_INT,&localA[0], chunck, MPI_INT,0, MPI_COMM_WORLD);

	traspose(localA, cols/numP, rows);

	double startTime = MPI_Wtime();
	matVetProduct(localY, localA, rows, cols/numP, localX);
	double endTime = MPI_Wtime();

	MPI_Reduce(&localY[0],&y[0],rows,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);

	double elapsed = endTime-startTime;
	double max;
	MPI_Reduce(&elapsed,&max,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

	if(rank == 0)
	{
		int i;

		if(rows < 10 && cols < 10)
		{
			for(i = 0; i < (cols); i++)
			{
				printf("%d \n",y[i]);

			}
		}
		printf("Elasped %lf\n",max);

	}








	MPI_Finalize();
	return 0;

}
/**
 * A tale funzione si passa la mtrice che si vule traposrre
 * in forma di array e il numero di colonne e di righe della matrice
 * trasposta
 */
void traspose(int* array, int cols, int rows)
{
	int i,j;
	int N = rows*cols;
	int* trasposeArray = (int *)malloc(sizeof(int)*N);
	int arrayIndex;
	int trasposeIndex;
	for(i = 0;i< rows ;i++)
	{
		for(j=0;j<cols;j++)
		{
			trasposeIndex = (i*cols)+j;
			arrayIndex = (j*rows)+i;
			//printf("tras %d, array %d\n",trasposeIndex,arrayIndex);
			trasposeArray[trasposeIndex] = array[arrayIndex];
		}

	}

	for(i = 0; i< N ;i++)
	{
		array[i]= trasposeArray[i];
	}

	free(trasposeArray);
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

