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
	cols = atoi(argv[1]);
	rows = atoi(argv[2]);

	int* sendCountX = (int*)malloc(sizeof(int)*numP);
	int* displacementsX = (int*)malloc(sizeof(int)*numP);
	int* sendCountA = (int*)malloc(sizeof(int)*numP);
	int* displacementsA = (int*)malloc(sizeof(int)*numP);
	int sendCountCol = 0;
	int N = rows*cols;
	int chunck = N/numP;
	int xChunck = cols/numP;
	int colChunck = cols/numP;
	int matrixRemainder = fmod(N,numP);
	int xRemainder = fmod(cols,numP);
	int colReminder =  fmod(cols,numP);
	int* localA;
	int* localX;
	int* localY = (int*)malloc(sizeof(int)*(rows));
	int* y = (int*)malloc(sizeof(int)*(rows));
	int* A = (int *)malloc(sizeof(int)*N);
	int* x = (int *)malloc(sizeof(int)*cols);


	int i = 0,sumX = 0,sumA = 0;

	for ( i = 0; i < numP; i++) {
		sendCountA[i] = chunck;
		if (matrixRemainder > 0) {
			sendCountA[i]++;
			matrixRemainder--;
		}


		displacementsA[i] = sumA;
		sumA += sendCountA[i];
	}



	for ( i = 0; i < numP; i++) {
		sendCountX[i] = xChunck;
		if (xRemainder > 0) {
			sendCountX[i]++;
			xRemainder--;
		}


		displacementsX[i] = sumX;
		sumX += sendCountX[i];
	}

	for ( i = 0; i < numP; i++) {
		sendCountCol = colChunck;
		if (colReminder > 0) {
			sendCountCol ++;
			colReminder--;
		}


		localX = (int*)malloc(sizeof(int)*sendCountX[rank]);
		localA = (int*)malloc(sizeof(int)*sendCountA[rank]);

		if(rank == 0)
		{

			int i,j;
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
			traspose(A, rows, cols);
		}

		MPI_Scatterv(&A[0],sendCountA,displacementsA,MPI_INT,&localA[0],sendCountA[rank],MPI_INT,0,MPI_COMM_WORLD);
		MPI_Scatterv(&x[0],sendCountX,displacementsX,MPI_INT,&localX[0],sendCountX[rank],MPI_INT,0,MPI_COMM_WORLD);


		printf("%d div  send count %d\n",sendCountA[rank],sendCountX[rank]);
		traspose(localA, sendCountX[rank], rows);
		matVetProduct(localY, localA, rows,  sendCountX[rank], localX);

		for(i = 0; i < (rows); i++)
		{
			//printf("%d ",localY[i]);

		}
		printf("\n");

		MPI_Reduce(&localY[0],&y[0],rows,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);


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

