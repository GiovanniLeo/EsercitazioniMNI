#define VS
//#define MGPU

#ifdef VS
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <device_functions.h>
#endif

#ifdef MGPU
#include<cuda.h>
#endif

#include <stdio.h>
#include<stdlib.h>

//#define DEBUG_MATRIX
#define DEBUG


//Dal profiler nvida per il kernel vengono usati  registri (si deve passare sulla multiGPU)Ese

void initializeArray(int *array, int N);
void printArray(int *array, int N);
__global__ void scalarProductv2GPU(int *a, int*b, int *c, int N);
int scalarProductCPU(int *a, int*b, int N);

int main()
{
	int *aHost, *bHost, *rHost;
	int *aDevice, *bDevice, *cDevice;
	int size, sizeSM, N, productCPU = 0, productGPU = 0, i;
	dim3 gridDim, blockDim;
	cudaEvent_t start, stop;
	float elapsed;


	printf("Inserisci la size degli array(N):");
	fflush(stdout);
	scanf("%d", &N);

	printf("Inserisci la size dei blocchi di thread (Nt):");
	fflush(stdout);
	scanf("%d", &blockDim.x);

	//Determino il numero esatto di blocchi 
	gridDim.x = N / blockDim.x + ((N % blockDim.x) == 0 ? 0 : 1);

	size = N * sizeof(int);

#ifdef DEBUG
	printf("Size della matrice: %d\n", N);
	printf("Numero totale di blocchi: %d\n", gridDim.x);
	printf("Numero totale dei Thread per blocco: %d\n", blockDim.x);
#endif 

	//Alloco la memoria sull' host
	aHost = (int*)malloc(size);
	bHost = (int*)malloc(size);
	rHost = (int*)calloc(N, sizeof(int)); //Azzeriamo gli array che raccolgono il risultato(lo inizializzo a zero)

	//Alloco memoria sul Device
	cudaMalloc((void **)&aDevice, size);
	cudaMalloc((void **)&bDevice, size);
	cudaMalloc((void **)&cDevice, gridDim.x * sizeof(int)); //C deve avere un numero di elemnti pari al numero di blocchi

	//Azzeriamo gli array che raccolgono il risultato
	cudaMemset(cDevice, 0, size);

	//inizializzo gli array
	initializeArray(aHost, N);
	initializeArray(bHost, N);

#ifdef DEBUG
	printArray(aHost, N);
	printArray(bHost, N);
#endif 

	//copio i dati dall'host al device
	cudaMemcpy(aDevice, aHost, size, cudaMemcpyHostToDevice);
	cudaMemcpy(bDevice, bHost, size, cudaMemcpyHostToDevice);

	//Somma parallela
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	sizeSM = blockDim.x * sizeof(int);
	scalarProductv2GPU << <gridDim, blockDim, sizeSM >> > (aDevice, bDevice, cDevice, N);

	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);

	cudaMemcpy(rHost, cDevice, gridDim.x * sizeof(int), cudaMemcpyDeviceToHost);

	for (i = 0; i < gridDim.x; i++)
	{
		productGPU += rHost[i];
	}


	cudaEventElapsedTime(&elapsed, start, stop);
	//De-allocazione eventi
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	printf("Tempo per prodotto scalare GPU:%f ms\n", elapsed);

	//Somma Seriale
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	productCPU = scalarProductCPU(aHost, bHost, N);
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);

	elapsed = 0;
	cudaEventElapsedTime(&elapsed, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	printf("Tempo per prodotto scalare CPU:%f ms\n", elapsed);
	//fare il metodo di controllo controllo 

	printf("Prodotto scalare CPU: %d\n", productCPU);
	printf("Prodotto scalare GPU: %d\n", productGPU);

	//De-allocazione eventi
	cudaEventDestroy(start); cudaEventDestroy(stop);
	//De-allocazione Host
	free(aHost); free(bHost); free(rHost);
	//De-allocazione Device
	cudaFree(aDevice); cudaFree(bDevice); cudaFree(cDevice);

}


void initializeArray(int *array, int N)
{
	int i;
	for (i = 0; i < N; i++)
	{
		array[i] = rand() % 5;
	}

}

void printArray(int *array, int N)
{
	int i;
	if (N < 20)
	{
		for (i = 0; i < N; i++)
		{
			printf("%d ", array[i]);
		}
		printf("\n");
	}
}

__global__ void scalarProductv2GPU(int *a, int*b, int *c, int N)
{

	extern __shared__ int s[];
	int index = threadIdx.x + blockIdx.x*blockDim.x;
	if (index < N)
		s[threadIdx.x] = a[index] * b[index];

	__syncthreads();
	for (unsigned int dist = blockDim.x/2; dist > 0; dist >>= 1) {
		if (threadIdx.x < dist) {
			s[threadIdx.x] += s[threadIdx.x + dist];
		}
		__syncthreads();
	}

	if (threadIdx.x == 0) {
		c[blockIdx.x] = s[threadIdx.x];
	}

}

int scalarProductCPU(int *a, int*b, int N)
{
	int i, product = 0;

	for (i = 0; i < N; i++)
	{
		product += a[i] * b[i];
	}

	return product;
}


