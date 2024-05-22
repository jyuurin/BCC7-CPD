#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void copiaValMaximo(int A[], int B[], int n, int p) 
{
		int i, j;
	
		#pragma omp parallel for private(i)
		for (i = 0; i < p; i++) 
		{
				B[p + i] = A[i * (n/p)];
		}

		for (i = 0; i < p; i++) 
		{
				for (j = 1; j < n/p; j++) 
				{
					if (A[i * (n/p) + j] > B[p + i])
						B[p + i] = A[i * (n/p) + j];
				}
		}
}

int main() 
{
	//entradas conforme o descritivo do slide
	int n = 16; 
	int p = 4; 
	int A[16] = {23, 37, 28, 33, 29, 32, 24, 31, 33, 27, 38, 23, 39, 22, 34, 21}; 
	int B[2 * 9]; 

	copiaValMaximo(A, B, n, p);

	int log_p = (int)log2(p);
	int j, i;

	for (j = log_p - 1; j >= 0; j--) 
	{
		#pragma omp parallel for private(i)
		for (i = (int)pow(2, j); i < (int)pow(2, j + 1); i++) 
		{
			B[i] = fmax(B[2 * i], B[2 * i + 1]);
		}
	}
	
	int maximo = B[1];
	printf("Valor Maximo: %d\n", maximo);

	return 0;
}
