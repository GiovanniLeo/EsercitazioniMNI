#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

#define DEBUG
#define DEBUGLOCALVECTOR

void generateMatVet(int *A, int *x, int matrixDim);
int matrixGetElement(int* A, int ROWS, int COLS, int r, int c);
void traspose(int* array, int cols, int rows);

int main (int argc, char* argv[])
{
	//Identificativi processi
	int rank,gridRank,colRank,rowRank;
	int *coordinate,colCoordinate,rowCoordinate;
	//dimensioni matrici e vettori
	int dim = 2,matrixDim,localDim[2],*ndim;
	int *period,reorder;
	int numP;
	MPI_Comm gridComm,colComm,rowComm;
	//strutture dati
	int *A,*x,*localX,*localA,*localASub;
	//MPI Inizialization
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&numP);

	ndim = (int*) malloc((dim)*sizeof(int));
	coordinate =  (int*) malloc((dim)*sizeof(int));

	matrixDim = atoi(argv[1]); //Dimensione matrice
	ndim[0] = atoi(argv[2]); //Righe di processori (p)
	ndim[1] = atoi(argv[3]); //Colonne di processori (q)

	//Dimensioni della matrice localA
	localDim[0] = matrixDim / ndim[0];
	localDim[1] = matrixDim / ndim[1];

	if(rank == 0)
	{

		A = (int*) malloc((matrixDim*matrixDim)*sizeof(int));
		x = (int*) malloc((matrixDim)*sizeof(int));
		generateMatVet(A, x, matrixDim);
	}

	period = (int *) malloc(dim*sizeof(int));
	//Vogliamo le righe non periodiche e i rank non devono essere riordinati
	period[0] = 0;
	period[1] = 0;
	reorder = 0;

	//Creazione griglia madre
	MPI_Cart_create(MPI_COMM_WORLD,dim,ndim,period,reorder,&gridComm);
	MPI_Comm_rank(gridComm,&gridRank);
	MPI_Cart_coords(gridComm, gridRank, dim, coordinate);

#ifdef DEBUG
	printf("rank %d cordinate (%d,%d)\n",rank,coordinate[0],coordinate[1]);
#endif

	int *rdim = (int*) malloc(dim*sizeof(int));
	//Creo il contesto delle colonne
	rdim[0] = 1; //righe variabili
	rdim[1] = 0; //colonne bloccate

	MPI_Cart_sub(gridComm,rdim,&colComm);
	MPI_Comm_rank(colComm,&colRank);
	MPI_Cart_coords(colComm,colRank,1,&colCoordinate);

	//Creo il contesto delle righe
	rdim[0] = 0; //righe bloccate
	rdim[1] = 1; //colonne variabili

	MPI_Cart_sub(gridComm,rdim,&rowComm);
	MPI_Comm_rank(rowComm,&rowRank);
	MPI_Cart_coords(rowComm,rowRank,1,&rowCoordinate);

	//Alloco il vettore localX poiche dopo devo distribuire x
	localX = (int*) malloc(localDim[1]*sizeof(int));

	//Titti i processori con coordinate (0,x) ricevono il vettore localX di dimensioni (localDim[1]*1)
	if(coordinate[0] == 0)
	{
		//Distribuiamo lungo il contesto delle righe
		MPI_Scatter(&x[0],localDim[1],MPI_INT,&localX[0],localDim[1],MPI_INT,0,rowComm);
	}
	//Spediamo localX lungo le colonne
	MPI_Bcast(&localX[0],localDim[1],MPI_INT,0,colComm);

#ifdef DEBUGLOCALVECTOR
	int i;
	printf("\n");
	for(i = 0; i < localDim[1]; i++)
	{
		printf("Coordinate (%d,%d) x value %d\n",coordinate[0],coordinate[1],localX[i]);
	}
#endif

#ifdef DEBUGLOCALVECTOR
	MPI_Barrier(MPI_COMM_WORLD);
#endif
	//Alloco la sottomatrice localA che dopo devo distribuire A
	localA = (int*) malloc((localDim[0]*matrixDim)*sizeof(int));
	localASub = (int*) malloc((localDim[0]*localDim[1])*sizeof(int));

	//Tutti i processori con coordinate (x,0) ricevono il vettore localA di dimensioni (localDim[0]*matrixDim)
	if(coordinate[1] == 0)
	{
		int N = localDim[0]*matrixDim;
		MPI_Scatter(&A[0],N,MPI_INT,&localA[0],N,MPI_INT,0,colComm);

#ifdef DEBUGLOCALVECTOR
		int i;
		printf("\n");
		if(rank == 0)
		{
			for(i = 0; i < N; i++)
			{
				printf("%d ",localA[i]);
			}
			printf("\n");
		}

#endif

		traspose(localA, localDim[0], matrixDim);



#ifdef DEBUGLOCALVECTOR
		printf("\n");
		if(rank == 0)
		{
			for(i = 0; i < N; i++)
			{
				printf("%d ",localA[i]);
			}
			printf("\n");
		}

#endif

	}

	//Devo ditribuire le sottomatrici di localA a tutti i processori lungo le righe i quali riceveranno una matrice (localDim[0]*localDim[1])
	MPI_Scatter(&localA[0],localDim[0]*localDim[1],MPI_INT,&localASub[0],localDim[0]*localDim[1],MPI_INT,0,rowComm);
	traspose(localASub, localDim[1], localDim[0]);

#ifdef DEBUGLOCALVECTOR
		printf("\n");
		if(rank == 0)
		{
			for(i = 0; i < localDim[0]*localDim[1]; i++)
			{
				printf("%d ",localASub[i]);
			}
			printf("\n");
		}

#endif






	MPI_Finalize();
	return 0;
}

void generateMatVet(int *A, int *x, int matrixDim)
{
	int i,j;
	int N = matrixDim * matrixDim;

	for(i = 0; i < N; i++)
	{
		A[i] = i + 1;
	}

	for(i = 0; i < matrixDim; i++)
	{
		x[i] = i;
	}

#ifdef DEBUG
	printf("A\n");
	for(i = 0;i< matrixDim;i++)
	{
		for(j=0;j<matrixDim;j++)
		{
			printf("%d ",matrixGetElement(A, matrixDim, matrixDim, j, i));
		}
		printf("\n");

	}
	printf("\nx\n");
	for(j=0;j<matrixDim;j++)
	{
		printf("%d \n",x[j]);
	}
	printf("\n");

#endif

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
			trasposeArray[trasposeIndex] = array[arrayIndex];
		}

	}

	for(i = 0; i< N ;i++)
	{
		array[i]= trasposeArray[i];
	}

	free(trasposeArray);
}
