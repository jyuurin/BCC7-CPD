#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void compare_e_troca(int vetor[], int i, int j) 
{
	if (vetor[i] > vetor[j]) 
	{
		int temp = vetor[i];
		vetor[i] = vetor[j];
		vetor[j] = temp;
	}
}

void bitonicSplit(int A[], int n) 
{
	if (n > 1) 
	{
		#pragma omp parallel for
		for (int i = 0; i <= (n - 2)/2; i++) 
		{
				compare_e_troca(A, i, i + n/2);
		}
		bitonicSplit(A, n/2);
		bitonicSplit(A + n/2, n/2);
	}
}

void reverse(int A[], int n) 
{
	#pragma omp parallel for
	for (int i = 0; i < n/2; i++) 
	{
		int temp = A[i];
		A[i] = A[n - i - 1];
		A[n - i - 1] = temp;
	}
}

void bitonicSort(int A[], int n) 
{
	if (n > 1) {
		bitonicSort(A, n/2);
		bitonicSort(A + n/2, n/2);
		reverse(A + n/2, n/2);
		bitonicSplit(A, n);
	}
}

int main() {
	int n = 8;
	int A[] = {3, 5, 8, 9, 7, 4, 2, 1};
	omp_set_num_threads(4);

	bitonicSort(A, n);

	printf("bitonic sort:\n");
	for (int i = 0; i < n; i++) 
	{
		printf("%d ", A[i]);
	}
	
	return 0;
}
