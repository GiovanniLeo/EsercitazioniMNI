#define VS
//#define MGPU

#ifdef VS
	#include "cuda_runtime.h"
	#include "device_launch_parameters.h"

#endif

#ifdef MGPU
	#include<cuda.h>
#endif

#include <stdio.h>
#include<stdlib.h>

//#define DEBUG_MATRIX
#define DEBUG


//Dal profiler nvida per il kernel vengono usati 13 registri (si deve passare sulla multiGPU)

void initializeMatrix(int *array, int matrixSize);
void printMatrix(int *matrix, int rows, int cols);
__global__ void sumMatrixGPU(int *a, int*b, int *c, int rows, int cols);
void sumMatrixCPU(int *a, int*b, int *c, int rows, int cols);
void matrixEqualCheck(int *mHost, int *mDevice, int cols, int rows);
int main()
{
	int *aHost, *bHost, *cHost, *rHost;
	int *aDevice, *bDevice, *cDevice;
	int size, N;
	dim3 gridDim, blockDim;
	int matrixSize;
	cudaEvent_t start, stop;
	float elapsed;


	printf("Inserisci la size della matrice quadrata(N):");
	fflush(stdout);
	scanf("%d",&N);
	
	printf("Inserisci la size dei blocchi di thread Nt (NtxNt):");
	fflush(stdout);
	scanf("%d", &blockDim.x);

	blockDim.y = blockDim.x;
	
	matrixSize = N * N;

	//Determino il numero esatto di blocchi 
	gridDim.x = N / blockDim.x + ((N % blockDim.x) == 0 ? 0 : 1);
	gridDim.y = N / blockDim.y + ((N % blockDim.y) == 0 ? 0 : 1);

	size = matrixSize * sizeof(int);

#ifdef DEBUG
	printf("Size della matrice: %d\n", matrixSize);
	printf("Numero totale di blocchi: %d (%d,%d)\n",gridDim.x*gridDim.y, gridDim.x, gridDim.y);
	printf("Numero totale dei Thread per blocco: %d (%d,%d)\n",blockDim.x*blockDim.y, blockDim.x, blockDim.y);
#endif 

	//Alloco la memoria sull' host
	aHost = (int*)malloc(size);
	bHost = (int*)malloc(size);
	rHost = (int*)malloc(size);
	cHost = (int*)calloc(matrixSize, sizeof(int)); //Azzeriamo gli array che raccolgono il risultato(lo inizializzo a zero)

	//Alloco memoria sul Device
	cudaMalloc((void **)&aDevice, size);
	cudaMalloc((void **)&bDevice, size);
	cudaMalloc((void **)&cDevice, size);

	//Azzeriamo gli array che raccolgono il risultato
	cudaMemset(cDevice, 0, size);

	//inizializzo le matrici
	initializeMatrix(aHost, matrixSize);
	initializeMatrix(bHost, matrixSize);

	//copio i dati dall'host al device
	cudaMemcpy(aDevice, aHost, size, cudaMemcpyHostToDevice);
	cudaMemcpy(bDevice, bHost, size, cudaMemcpyHostToDevice);

	//Somma parallela
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	sumMatrixGPU <<<gridDim,blockDim>>> (aDevice, bDevice, cDevice, N, N);
	
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);

	cudaEventElapsedTime(&elapsed, start, stop);
	//De-allocazione eventi
	cudaEventDestroy(start); 
	cudaEventDestroy(stop);
	
	printf("Tempo per la somma di matrici GPU:%f ms\n", elapsed);

	//Somma Seriale
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	cudaEventRecord(start, 0);

	sumMatrixCPU(aHost, bHost, cHost, N, N);
	
	cudaEventRecord(stop, 0);
	cudaEventSynchronize(stop);

	elapsed = 0;
	cudaEventElapsedTime(&elapsed, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	printf("Tempo per la somma di matrici CPU:%f ms\n", elapsed);
	//fare il metodo di controllo controllo 
	cudaMemcpy(rHost, cDevice, size, cudaMemcpyDeviceToHost);
#ifdef DEBUG_MATRIX
	printf("Host\n");
	printMatrix(cHost, N, N);
	printf("Device\n");
	printMatrix(rHost, N, N);
#endif 
	
	matrixEqualCheck(cHost,rHost,N,N);

	//De-allocazione eventi
	cudaEventDestroy(start); cudaEventDestroy(stop);
	//De-allocazione Host
	free(aHost); free(bHost); free(cHost); free(rHost);
	//De-allocazione Device
	cudaFree(aDevice); cudaFree(bDevice); cudaFree(cDevice);

}


void initializeMatrix(int *array, int matrixSize)
{
	int i;
	for ( i = 0; i < matrixSize; i++)
	{
		array[i] = i;
	}
	
}

__global__ void sumMatrixGPU(int *a, int*b, int *c, int rows, int cols)
{
	//definire id per il controllo
	int idx = (blockDim.x*blockIdx.x) + threadIdx.x;
	int idy = (blockDim.y*blockIdx.y) + threadIdx.y;
	if (idx < rows && idy < cols)
	{
		//Essendo che rows e cols sono uguli li possiamo usare tranquillamente in maniera intercambiabile
		c[(idx*cols) + idy] = a[(idx*cols) + idy] + b[(idx*cols) + idy];
	}
	
}

void printMatrix(int *matrix, int rows, int cols)
{
	int i, j;

	if (cols < 10 && rows < 10)
	{
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				printf("%d ", matrix[(i*cols) + j]);
			}
			printf("\n");
		}
		printf("\n");
	}
	printf("\n");

}

void sumMatrixCPU(int *a, int*b, int *c, int rows, int cols)
{
	int i, size;
	size = rows * cols;
	for (i = 0; i < size; i++)
	{
		c[i] = a[i] + b[i];
	}
}

void matrixEqualCheck(int *mHost, int *mDevice, int cols, int rows)
{
	int i, size, count = 0;
	size = cols * rows;

	for ( i = 0; i < size; i++)
	{
		if (mHost[i] == mDevice[i])
		{
			count++;
		}
	}

	if (count == size)
	{
		printf("Le matrici sono uguali\n");
	}
	else
	{
		printf("Le matrici sono diverse\n");
	}
}
