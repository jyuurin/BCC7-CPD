#include <stdio.h>
#include <omp.h>

int buscaBinaria(int num, int vetor[], int i, int f) 
{
	int inicio = i;
	int fim = f;
	while (inicio <= fim) 
	{
		int meio = (inicio + fim) / 2;
		if (num < vetor[meio])
			fim = meio - 1;
		else if (num > vetor[meio])
			inicio = meio + 1;
		else
			return meio;
	}
	return inicio;
}

void merge(int A[], int B[], int C[], int n) 
{
	int posicaoEmA[n/2], posicaoEmB[n/2];
	int i;

	#pragma omp parallel for
	for (i = 0; i < n/2; i++) 
	{
		posicaoEmA[i] = buscaBinaria(B[i], A, 0, n/2 - 1);
		posicaoEmB[i] = buscaBinaria(A[i], B, 0, n/2 - 1);
	}

	#pragma omp parallel for
	for (i = 0; i < n/2; i++) 
	{
		C[posicaoEmA[i] + i] = B[i];
		C[posicaoEmB[i] + i] = A[i];
	}
}

int main() 
{
	int n = 8;
	int A[] = {13, 14, 17, 19};
	int B[] = {11, 15, 16, 20};
	int C[n];

	merge(A, B, C, n);

	printf("Merge: ");
	for (int i = 0; i < n; i++) 
	{
		printf("%d ", C[i]);
	}
	return 0;
}
