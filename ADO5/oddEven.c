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

//merge de valores, par e impar
void oddEvenMerge(int A[], int B[], int C[], int n)
{
	if (n == 1) 
	{
		C[0] = A[0];
		C[1] = B[0];
		compare_e_troca(C, 0, 1);
	} 
	else 
	{
		//valores pares e ímpares
		int odd_A[n/2], odd_B[n/2], even_A[n/2], even_B[n/2];

		#pragma omp parallel for
		for (int i = 0; i < n/2; i++) 
		{
			odd_A[i] = A[2*i+1];
			odd_B[i] = B[2*i+1];
			even_A[i] = A[2*i];
			even_B[i] = B[2*i];
		}
		
		//merge de elementos recursivo em paralelo
		int odd[n], even[n];
		#pragma omp task
		oddEvenMerge(odd_A, odd_B, odd, n/2);
		#pragma omp task
		oddEvenMerge(even_A, even_B, even, n/2);
		#pragma omp taskwait
		//combinando valores, conforme odd-even
		#pragma omp parallel for
		for (int i = 0; i < (n - 2)/2; i++) 
		{
			C[2*i] = even[i];
			C[2*i + 1] = odd[i];
		}
		#pragma omp parallel for
		for (int i = 0; i < (n - 2)/2 - 1; i++) 
		{
			compare_e_troca(C, 2*i + 1, 2*i + 2);
		}
	}
}


void oddEvenMergeSort(int A[], int n) {
	if (n > 1) 
	{
		oddEvenMergeSort(A, n/2);
		oddEvenMergeSort(A + n/2, n/2);
		int C[n];
		oddEvenMerge(A, A + n/2, C, n);
		for (int i = 0; i < n; i++) 
		{
				A[i] = C[i];
		}
	}
}

int main() {
	int n = 8;
	int A[] = {2, 4, 5, 8, 1, 3, 6, 7};
	omp_set_num_threads(4);

	oddEvenMergeSort(A, n);

	printf("resultado odd-even:\n");
	for (int i = 0; i < n; i++) 
	{
		printf("%d ", A[i]);
	}
	return 0;
}
