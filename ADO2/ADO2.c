#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
	//entradas conforme o descritivo do slide
	int n = 12;
	int pai[12] = {9, 0, 4, 8, 3, 2, 7, 5, 10, 1, 11, 6}; 
	int peso[12] = {2, 3, 1, 2, 1, 4, 3, 1, 3, 2, 1, 4}; 
	int soma[n];
	int p[n];
	int pesoAux[n];

	//copias de pai e peso para os vetores auxiliares, em paralelo.
	#pragma omp parallel for
	for (int i = 0; i < n; i++) {
			p[i] = pai[i];
				pesoAux[i] = peso[i];
	}

	//soma de prefixos
	#pragma omp parallel for
	for (int i = 0; i < n; i++) 
	{
		while (p[i] != -1) 
		{
			pesoAux[i] += pesoAux[p[i]];
			p[i] = p[p[i]];
		}
		soma[i] = pesoAux[i];
	}
	
	printf("soma dos pesos:\n");
	for (int i = 0; i < n; i++) 
	{
		printf("soma[%d] = %d\n", i, soma[i]);
	}

	return 0;
}
