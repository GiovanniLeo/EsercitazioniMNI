
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<stdlib.h>
#include <stdio.h>
#include<cuda.h>

void initializeArray(int*, int);
void stampaArray(int*, int);
void equalArray(int*, int*, int);
void prodottoArrayCompPerCompCPU(int *, int *, int *, int);
__global__ void prodottoArrayCompPerCompGPU(int*, int*, int*, int);

int main(int argn, char * argv[])
{
	//numero di blocchi e numero di thread per blocco
	dim3 gridDim, blockDim;
	int N; //numero totale di elementi dell'array
	//array memorizzati sull'host
	int *A_host, *B_host, *C_host;
	//array memorizzati sul device
	int *A_device, *B_device, *C_device;
	int *copy; //array in cui copieremo i risultati di C_device
	int size; //size in byte di ciascun array

	printf("***\t PRODOTTO COMPONENTE PER COMPONENTE DI DUE ARRAY \t***\n");
	/* se l'utente non ha inserito un numero sufficiente di
	parametri da riga di comando, si ricorre ai valori di
	default per impostare il numero di thread per blocco, il
	numero totale di elementi e il flag di stampa */
	printf("***\t PRODOTTO COMPONENTE PER COMPONENTE DI DUE ARRAY \t***\n");
	printf("Inserisci il numero elementi dei vettori\n");
	scanf("%d", &N);
	printf("Inserisci il numero di thread per blocco\n");
	scanf("%d", &blockDim);


	//determinazione esatta del numero di blocchi
	gridDim = N / blockDim.x +
		((N%blockDim.x) == 0 ? 0 : 1);
	//size in byte di ogni array
	size = N * sizeof(int);

	//stampa delle info sull'esecuzione del kernel
	printf("Numero di elementi = %d\n", N);
	printf("Numero di thread per blocco = %d\n",
		blockDim.x);
	printf("Numero di blocchi = %d\n", gridDim.x);

	//allocazione dati sull'host
	A_host = (int*)malloc(size);
	B_host = (int*)malloc(size);
	//lo inizializzo a zero
	C_host = (int*) calloc(N, sizeof(int));
	copy = (int*)malloc(size);
	//allocazione dati sul device
	cudaMalloc((void**)&A_device, size);
	cudaMalloc((void**)&B_device, size);
	cudaMalloc((void**)&C_device, size);

	//inizializzazione dati sull'host
	initializeArray(A_host, N);
	initializeArray(B_host, N);



	//copia dei dati dall'host al device
	cudaMemcpy(A_device, A_host, size, cudaMemcpyHostToDevice);
	cudaMemcpy(B_device, B_host, size, cudaMemcpyHostToDevice);

	//azzeriamo il contenuto della matrice C
	
	cudaMemset(C_device, 0, size);
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	cudaEventRecord(start);
	//invocazione del kernel
	prodottoArrayCompPerCompGPU << <gridDim, blockDim >> >
		(A_device, B_device, C_device, N);
	cudaEventRecord(stop);
	cudaEventSynchronize(stop); // assicura che tutti siano arrivati all'evento stop prima di registrare il tempo
	float elapsed;
	// tempo tra i due eventi in millisecondi
	cudaEventElapsedTime(&elapsed, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	//copia dei risultati dal device all'host
	cudaMemcpy(copy, C_device, size, cudaMemcpyDeviceToHost);

	printf("tempo GPU=%f\n", elapsed);


	// calcolo su CPU
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start);
	//chiamata alla funzione seriale per il prodotto di due array
	prodottoArrayCompPerCompCPU(A_host, B_host, C_host, N);
	cudaEventRecord(stop);
	cudaEventSynchronize(stop); // assicura che tutti siano arrivati all'evento stop prima di registrare il tempo
	cudaEventElapsedTime(&elapsed, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	printf("tempo CPU=%f\n", elapsed);


	//stampa degli array e dei risultati
	if (N < 20)
	{
		printf("array A\n"); stampaArray(A_host, N);
		printf("array B\n"); stampaArray(B_host, N);
		printf("Risultati host\n"); stampaArray(C_host, N);
		printf("Risultati device\n"); stampaArray(copy, N);
	}

	//test di correttezza
	//equalArray(copy, C_host, N);



	//de-allocazione host
	free(A_host);
	free(B_host);
	free(C_host);
	free(copy);
	//de-allocazione device
	cudaFree(A_device);
	cudaFree(B_device);
	cudaFree(C_device);
	return 0;
}

void initializeArray(int *array, int n)
{
	int i;
	for (i = 0; i < n; i++)
		array[i] = i;
}
void stampaArray(int* array, int n)
{
	int i;
	for (i = 0; i < n; i++)
		printf("%d ", array[i]);
	printf("\n");
}
void equalArray(int* a, int*b, int n)
{
	int i = 0;
	while (a[i] == b[i])
		i++;
	if (i < n)
		printf("I risultati dell'host e del device sono diversi\n");
	else
		printf("I risultati dell'host e del device coincidono\n");
}

//Seriale
void prodottoArrayCompPerCompCPU
(int *a, int *b, int *c, int n)
{
	int i;
	for (i = 0; i < n; i++)
		c[i] = a[i] * b[i];
}
//Parallelo
__global__ void prodottoArrayCompPerCompGPU
(int* a, int* b, int* c, int n)
{
	int index = threadIdx.x + blockIdx.x*blockDim.x;
	if (index < n)
		c[index] = a[index] * b[index];
}
