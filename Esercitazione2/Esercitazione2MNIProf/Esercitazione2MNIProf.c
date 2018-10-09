#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"

/**
 * Funzione per allocare lo spazio per una matrice
 * ROWS  numero di righe
 * COLS  numero di colonne
 */
double* get_matrix_space(int ROWS, int COLS)
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
double matrix_get(double* A, int ROWS, int COLS, int r, int c)
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
void  matrix_set(double* A, int ROWS, int COLS, int r, int c, double v)
{
	A[(r * COLS) + c] = v;
}

/**
 * Esegue il prodotto matrice vettore
 */
void prod_mat_vett(double w[], double *a, int ROWS, int COLS, double v[])
{
	int i, j;

	for(i=0;i<ROWS;i++)
	{
		w[i]=0;
		for(j=0;j<COLS;j++)
		{
			w[i] += matrix_get(a, ROWS, COLS, i, j) * v[j];
		}
	}
}


int main(int argc, char **argv) {

	int nproc;              // Numero di processi totale
	int me;                 // Il mio id
	int n;                  // Dimensione della matrice
	int local_n;            // Dimensione dei dati locali
	int i,j;                    // Iteratori vari

	// Variabili di lavoro
	double* A;
	double* v;
	double* localA;
	double* local_w;
	double* w;

	/*Attiva MPI*/
	MPI_Init(&argc, &argv);
	/*Trova il numero totale dei processi*/
	MPI_Comm_size (MPI_COMM_WORLD, &nproc);
	/*Da ad ogni processo il proprio numero identificativo*/
	MPI_Comm_rank (MPI_COMM_WORLD, &me);

	// Se sono a radice
	if(me == 0)
	{

		n = atoi(argv[1]) * 1000;
		// Porzione di dati da processare
		local_n = n/nproc;

		// Calcolo la matrice A e alloco v e w
		A = get_matrix_space(n,n);
		v = malloc(sizeof(double)*n);
		w =  malloc(sizeof(double)*n);


		for (i=0;i<n;i++)
		{
			v[i]=i;
			for(j=0;j<n;j++)
			{
				if (j==0)
					matrix_set(A, n, n, i, j, (1.0/(i+1))-1);
				else
					matrix_set(A, n, n, i, j, (1.0/(i+1))-(pow(1.0/2.0,j)));

			}

		}

		if(n < 50)
		{
			printf("A = \n");
			for (i=0;i<n;i++)
			{
				for(j=0;j<n;j++)
				{
					printf("%f ", matrix_get(A, n, n, i, j) );
				}
				printf("\n");
			}


			printf("v = \n");
			for (i=0;i<n;i++)
			{
				printf("%f\n", v[i]);
			}
			printf("\n");
		}

	}


	// Spedisco n e local_v
	MPI_Bcast(&n,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&local_n,1,MPI_INT,0,MPI_COMM_WORLD);

	// Se sono un figlio alloco v
	if(me != 0)
		v = malloc(sizeof(double)*n);

	MPI_Bcast(&v[0],n,MPI_DOUBLE,0,MPI_COMM_WORLD);

	// tutti allocano A locale e il vettore dei risultati
	localA  = get_matrix_space(local_n, n);
	local_w = malloc(local_n * sizeof(double));

	// Adesso 0 invia a tutti un pezzo della matrice
	int num = local_n*n;
	MPI_Scatter(
			&A[0], num, MPI_DOUBLE,
			&localA[0], num, MPI_DOUBLE,
			0, MPI_COMM_WORLD);

	// Scriviamo la matrice locale ricevuta
	if(local_n < 10)
	{
		printf("localA %d = \n", me);
		for(i = 0; i < local_n; i++)
		{
			for(j = 0; j < n; j++)
				printf("%lf\t", matrix_get(localA, local_n, n, i, j));
			printf("\n");
		}
	}

	double startTime = MPI_Wtime();

	// Effettuiamo i calcoli
	prod_mat_vett(local_w,localA,local_n,n,v);

	double endTime = MPI_Wtime();
	double elapsed = endTime-startTime;
	double max;
	MPI_Reduce(&elapsed,&max,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);


	// 0 raccoglie i risultati parziali
	MPI_Gather(&local_w[0],local_n,MPI_DOUBLE,&w[0],local_n,MPI_DOUBLE,0,MPI_COMM_WORLD);

	// 0 stampa la soluzione
	if(me==0 && n < 50)
	{
		printf("w = \n");
		for(i = 0; i < n; i++)
			printf("%f ", w[i]);
		printf("\n");
	}

	if (me == 0)
	{
		printf("Elapsed time %lf\n",max);
		printf("===================================\n");
	}

	MPI_Finalize (); /* Disattiva MPI */
	return 0;
}
