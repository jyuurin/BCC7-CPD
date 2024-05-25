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

	for (int i = 0; i < r; i++) {
		localVet[i] = (rank * r) + i + 1;
		localSoma += localVet[i];
	}
	
	int esq = (rank == 0) ? (tamanho - 1) : (rank - 1);
	int dir = (rank + 1) % tamanho;

	if (rank == 0) {
		MPI_Send(&localSoma, 1, MPI_INT, dir, 0, MPI_COMM_WORLD);
		MPI_Recv(&totalSoma, 1, MPI_INT, esq, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	} else {
		MPI_Recv(&totalSoma, 1, MPI_INT, esq, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		localSoma += totalSoma;
		MPI_Send(&localSoma, 1, MPI_INT, dir, 0, MPI_COMM_WORLD);
	}

	if (rank == 0) 
		printf("Total: %d\n", totalSoma);

	free(localVet);
	return 0;
}
