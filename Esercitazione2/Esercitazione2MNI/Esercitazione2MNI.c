/*
 ============================================================================
 Name        : Esercitazione2MNI
 Author      : Giovanni Leo
 Number      : 0522500538
 Description :Esercitazione2MNI
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

double* setMatrixSpace(int ROWS, int COLS);
double matrixGetElement(double* A, int ROWS, int COLS, int r, int c);
void  matrixSetElement(double* A, int ROWS, int COLS, int r, int c, double v);
void   matVetProduct(double w[], double *a, int ROWS, int COLS, double v[]);

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

	int* sendCount;
	int* displacements;
	double* A =  setMatrixSpace(rows, cols);
	double* recivedBuffer;

	//Vettore a cui si deve moltiplicare la matrice
	double* x = (double*)malloc(sizeof(double)*cols);
	double* y = (double*)malloc(sizeof(double)*cols);
	double* localY;

	sendCount = (int*)malloc(sizeof(int)*numP);
	displacements = (int*)malloc(sizeof(int)*numP);

	int matrixDim = rows * cols;
	int rowsChunck = matrixDim/ numP;
	int  rowsRemainder = fmod(matrixDim,numP);
	//printf("row start %d, row end = %d, %d\n", rowsStart, rowsStop,(rowsStop - rowsStart));

	int i = 0,sum = 0;

	/**
	 * Creare un array di chuck in base al resto ed un array di displacement al
	 *  fine di trovare gli elementi della matrice tali array servono alla scatterv
	 *  la quale in base al processore andrà a recuperare l'indice e valore in send count e displacement
	 *  in modo tale da prebndere i giusti valori nell'array
	 */
	for ( i = 0; i < numP; i++) {
		sendCount[i] = rowsChunck;
		if (rowsRemainder > 0) {
			sendCount[i]++;
			rowsRemainder--;
		}

		displacements[i] = sum;
		sum += sendCount[i];
	}

	//printf("rank = %d, send count = %d, displacement = %d\n", rank, sendCount[rank],displacements[rank]);
	recivedBuffer = setMatrixSpace(sendCount[rank], cols);
	localY = setMatrixSpace(sendCount[rank], cols);

	//inializzazione x
	for(i = 0; i < cols ; i++)
	{
		x[i] = 1;
	}
	// Inizializzazione matrice
	if(rank == 0)
	{
		int i,j;
		for(i = 0; i < rows ; i++)
		{
			for(j = 0; j < cols ; j++)
			{
				matrixSetElement(A, rows, cols, i, j,1);
				//printf("%lf ", matrixGetElement(A, rows, cols, i, j) );
			}
			//printf("\n");
		}
	}

	MPI_Scatterv(&A[0],sendCount,displacements,MPI_DOUBLE,&recivedBuffer[0],sendCount[rank],MPI_DOUBLE,0,MPI_COMM_WORLD);


	matVetProduct(localY, recivedBuffer, sendCount[rank], cols, x);
	MPI_Gatherv(localY,sendCount[rank],MPI_DOUBLE,y,sendCount,displacements,MPI_DOUBLE,0,MPI_COMM_WORLD);


	if(rank==0)
	{
		printf("Prodotto\n");
		for(i = 0; i < cols; i++)
			printf("%f ", y[i]);
		printf("\n");
	}

	MPI_Finalize();
	return 0;

}

/**
 * Funzione per allocare lo spazio per una matrice
 * ROWS  numero di righe
 * COLS  numero di colonne
 */
double* setMatrixSpace(int ROWS, int COLS)
{
	double* A = malloc(ROWS * COLS * sizeof(double));
	return A;
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
	return A[ (r * COLS) + c ];
}

/**
 * Funzione per impostare un elemento della matrice.
 * A     puntatore alla matrice
 * ROWS  numero di righe della matrice
 * COLS  numero di colonne della matrice
 * r     riga dell'elemento da impostare
 * c     colonna dell'elemento da impostare
 * v     valore da impostare
 */
void  matrixSetElement(double* A, int ROWS, int COLS, int r, int c, double v)
{
	A[(r * COLS) + c] = v;
}

/**
 * Esegue il prodotto matrice vettore
 */
void matVetProduct(double w[], double *a, int ROWS, int COLS, double v[])
{
	int i, j;

	for(i=0;i<ROWS;i++)
	{
		w[i]=0;
		for(j=0;j<COLS;j++)
		{
			w[i] += a[i] * v[j];
		}

	}
}
