#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
	int rank, tamanho;
	int n = 12;
	int r;
	int *localVet;
	int localSoma = 0;
	int totalSoma = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &tamanho);

	r = n / tamanho;
	localVet = (int *)malloc(r * sizeof(int));

// Inicializa o sub-vetor local
	for (int i = 0; i < r; i++) 
	{
		localVet[i] = (rank * r) + i + 1;
		localSoma += localVet[i];
	}

	int raiz = 0;
	int somaGeral = 0;

	MPI_Reduce(&localSoma, &somaGeral, 1, MPI_INT, MPI_SUM, raiz, MPI_COMM_WORLD);

	if (rank == raiz) 
	{
		printf("Soma total: %d\n", somaGeral);
	}

	free(localVet);
	return 0;
}
