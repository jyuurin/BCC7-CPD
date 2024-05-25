#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
	int rank, tamanho;
	int n = 12;
	int r;
	int *localVet;
	int *somaPrefix;
	int localSoma = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &tamanho);

	r = n / tamanho;
	localVet = (int *)malloc(r * sizeof(int));
	somaPrefix = (int *)malloc(r * sizeof(int));

	for (int i = 0; i < r; i++) 
	{
		localVet[i] = (rank * r) + i + 1;
		localSoma += localVet[i];
	}

	somaPrefix[0] = localVet[0];
	for (int i = 1; i < r; i++) 
	{
			somaPrefix[i] = somaPrefix[i - 1] + localVet[i];
	}

	for (int i = 0; i < tamanho; i++) 
	{
		if (i != rank)
			MPI_Send(&localVet[r - 1], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}

	int somaTotal = localSoma;
	for (int i = 0; i < tamanho; i++) 
	{
		if (i != rank) 
		{
			int recPrefix;
			MPI_Recv(&recPrefix, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			somaTotal += recPrefix;
		}
	}
	int prefixo = somaTotal - localSoma;
	for (int i = 0; i < r; i++) 
	{
		somaPrefix[i] += prefixo;
	}

	printf("Processador %d:\n Prefixos:\n", rank);
	for (int i = 0; i < r; i++) 
	{
		printf("%d", somaPrefix[i]);
	}

	free(localVet);
	free(somaPrefix);
	return 0;
}
