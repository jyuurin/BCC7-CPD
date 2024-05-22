#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main() {
	//entradas conforme o descritivo do slide
	int n = 6;
	int pai[] = {-1, 0, 1, 2, 3, 0}; 
	int peso[] = {1, 2, 3, 4, 5, 6}; 
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
