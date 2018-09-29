#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int main (int argc , char* argv[])
{
	clock_t  start;
	clock_t end;
	int lenghtMult = atoi(argv[1]);
	int arrayLenght = 10000000 * lenghtMult;
	int* array = (int *)malloc(sizeof(int)*arrayLenght);

	start = clock();
	int i = 0;
	int sum = 0;
	for(i = 0; i < arrayLenght; i++)
	{
		array[i] = rand()%20+1;
		sum+=array[i];
	}

	printf("The sum is %d\n",sum);

	end = clock();

	double finalTime = (double)(end- start)/CLOCKS_PER_SEC;
	printf("Elapsed time:%lf\n",finalTime);
}
