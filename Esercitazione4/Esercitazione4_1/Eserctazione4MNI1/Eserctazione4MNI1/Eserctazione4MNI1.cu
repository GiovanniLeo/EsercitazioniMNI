
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<stdlib.h>
#include <stdio.h>

void inizializeArray(int *array, int dim);
void printArray(int *array, int dim);
__global__ void arrayProductGPU(int *a, int *b, int *c, int dim);


//#define DEBUG

int main(int argc, char* argv[])
{
	dim3 gridDim, blockDim; //Dimensione griglia e dimensione blocco
	int N; //Elementi array
	int *aHost, *bHost, *cHost; //ArrayCPU
	int *aDevice, *bDevice, *cDevice; //ArrayGPU
	int *temp;
	int size, sum = 0;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	
	printf("Inserisci il numero di elenti del vettore:");
	fflush(stdout);
	scanf("%d", &N);
	
	blockDim.x = 32;
	gridDim.x = N / blockDim.x + ((N % blockDim.x) == 0 ? 0 : 1); //serve a determinare quanti blocchi c sono esattamente
	size = sizeof(int)*N;

#ifdef DEBUG
	printf("Numero di blocchi: %d\n", gridDim.x);
	printf("Numero di elementi array: %d\n", N);
	printf("Numero di thread per blocco: %d\n", blockDim.x);
#endif  

	aHost = (int*)malloc(size);
	bHost = (int*)malloc(size);
	cHost = (int*)calloc(N,sizeof(int));
	temp = (int*)malloc(size);

	cudaMalloc((void**)&aDevice, size);
	cudaMalloc((void**)&bDevice, size);
	cudaMalloc((void**)&cDevice, size);


	inizializeArray(aHost, N);
	inizializeArray(bHost, N);
	cudaMemset(cDevice, 0, size);

#ifdef DEBUG
	printArray(aHost, N);
#endif

	cudaMemcpy(aDevice, aHost, size ,cudaMemcpyHostToDevice);
	cudaMemcpy(bDevice, bHost, size, cudaMemcpyHostToDevice);
	
	cudaEventRecord(start,0);
	arrayProductGPU <<<gridDim, blockDim>>> (aDevice, bDevice, cDevice, N);
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);

	float elapsed;
	cudaEventElapsedTime(&elapsed,start, stop);
	printf("Il prototto è stato eseguito in :%f ms\n", elapsed);

	cudaMemcpy(cHost, cDevice, size, cudaMemcpyDeviceToHost);

#ifdef DEBUG
	printArray(cHost, N);
#endif

	int i;
	for (i = 0; i < N; i++)
	{
		sum += cHost[i];
	}
	printf("La somma è :%d\n", sum);

	cudaEventDestroy(start);
	cudaEventDestroy(stop);
	cudaFree(aDevice);
	cudaFree(bDevice);
	cudaFree(cDevice);
	free(aHost);
	free(bHost);
	free(cHost);







 
    return 0;
}

void inizializeArray(int *array, int dim)
{
	int i;
	for (i = 0; i < dim; i++)
	{
		array[i] = i;
	}
}

void printArray(int *array, int dim)
{
	int i;
	for (i = 0; i < dim; i++)
	{
		printf("%d ", array[i]);
	}
	printf("\n");
}

/*
	*a: primo elemento della moltiplicazione
	*b: secondo elemento della moltiplicazione
	*c: array contenete il risultato della moltiplicazione
	dim: dimensione degli array
*/

__global__ void arrayProductGPU(int *a, int *b, int *c, int dim)
{
	int index = (blockDim.x * blockIdx.x) + threadIdx.x;
	if (index < dim) //Facciamo questo controllo poichè potrebbero essereci thre thread che non hanno associato nessun indice dell'array
	{
		c[index] = a[index] * b[index];
	}
}

