
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdlib.h>
#include <stdio.h>

#define DEBUG

void initializeMatrix(int *matrix, int rows, int cols);
void printMatrix(int *matrix, int rows, int cols);
__global__ void matrixSumGpu(int *a, int *b, int *c, int rows, int cols);


int main()
{
	int rows, cols;
	int matrixDim;
	int *aHost, *bHost,*cHost;
	int *aDevice, *bDevice,*cDevice;
	int size;
	dim3 gridDim, blockDim;
	cudaEvent_t start, stop;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);

	

	printf("Inserire le dimensioni della matrice NxN(rows cols):");
	fflush(stdout);
	scanf("%d %d", &rows, &cols);

	matrixDim = rows * cols;
	size = matrixDim * sizeof(int);

	blockDim.x = 4; blockDim.y = 8; //blocchi di 32 thread
	gridDim.x = cols / blockDim.x + ((cols % blockDim.x) == 0 ? 0 : 1);
	gridDim.y = rows / blockDim.y + ((rows % blockDim.y) == 0 ? 0 : 1);

#ifdef DEBUG
	printf("Numero di blocchi lungo asse x: %d Numero di blocchi lungo asse y:%d\n", gridDim.x, gridDim.y);
	printf("Numero di elementi della matrice : %d\n", matrixDim);
	printf("Numero di thread lungo asse x: %d Numero di thread lungo asse y:%d\n", blockDim.x,blockDim.y);
#endif  

	aHost = (int*)malloc(size);
	bHost = (int*)malloc(size);
	cHost = (int*)calloc(matrixDim, sizeof(int));


	cudaMalloc((void**)&aDevice, size);
	cudaMalloc((void**)&bDevice, size);
	cudaMalloc((void**)&cDevice, size);

	initializeMatrix(aHost, rows, cols);
	initializeMatrix(bHost, rows, cols);
	cudaMemset(cDevice, 0, size);

#ifdef DEBUG
	printMatrix(aHost, rows, cols);
	printMatrix(bHost, rows, cols);
#endif
	cudaMemcpy(aDevice, aHost, size, cudaMemcpyHostToDevice);
	cudaMemcpy(bDevice, bHost, size, cudaMemcpyHostToDevice);

	cudaEventRecord(start,0);
	matrixSumGpu <<<gridDim, blockDim >>> (aDevice, bDevice, cDevice, rows, cols);
	cudaEventRecord(stop,0);
	cudaEventSynchronize(stop);
	
	float elasped;
	cudaEventElapsedTime(&elasped, start, stop);
	printf("Tempo per la somma di matrici :%f ms\n",elasped);

	cudaMemcpy(cHost, cDevice, size, cudaMemcpyDeviceToHost);

#ifdef DEBUG
	printMatrix(cHost, rows, cols);
#endif

   

    return 0;
}

void initializeMatrix(int *matrix, int rows, int cols)
{
	int i, j;

		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < cols; j++)
			{
				matrix[(i*cols) + j] = (i*rows)+j;
			}
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

}

/*
	*a:   primo elemento della somma
	*b:   secondo elemento della somma
	*c:	  array contenete il risultato della somma
	cols: numero della colonne
	rows: numero delle righe
*/
__global__ void matrixSumGpu(int *a, int *b, int *c, int rows, int cols)
{
	int i, j;
	//definire id per il controllo
	int idx = (blockDim.x*blockIdx.x) + threadIdx.x;
	int idy = (blockDim.y*blockIdx.y) + threadIdx.y;

	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			if (idx < cols && idy < rows)
			{
				c[(i*cols) + j] = a[(i*cols) + j] + b[(i*cols) + j];
			}
			
		}
	}
}
