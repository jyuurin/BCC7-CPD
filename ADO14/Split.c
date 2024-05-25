#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TAMMAX 16

int Compar(const void *a, const void *b) 
{
	int left = *(const int *)a;
	int right = *(const int *)b;
	return (left - right);
}

int main(int argc, char **argv) 
{
	int rank, size, tam, tamrbuf, tamB, i, j, k;
	int *VetorDados = {3,20,51,12,35,7,10,40,4,23,1,9,6,8,21,2};
	int *SubVetor = (int *)malloc(TAMMAX * sizeof(int));
	int *Quartis = (int *)malloc((TAMMAX-1) * sizeof(int));
	int *S, *sbuf, *send_displ, *BuckVetor, *rbuf, *rec_displ;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	tam = TAMMAX / size;

	MPI_Scatter(VetorDados, tam, MPI_INT, SubVetor, tam, MPI_INT, 0, MPI_COMM_WORLD);

	qsort(SubVetor, tam, sizeof(int), Compar);

	for (i = 1; i < size; i++) 
	{
		Quartis[i-1] = (SubVetor[((i*tam)/size) - 1] + SubVetor[((i*tam)/size)]) / 2;
	}

	MPI_Gather(Quartis, size-1, MPI_INT, rbuf, size-1, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0) 
	{
		tamrbuf = size * (size - 1);
		qsort(rbuf, tamrbuf, sizeof(int), Compar);
		S = (int *)malloc(size * sizeof(int));
		for (i = 1; i < size; i++) {
				S[i-1] = (rbuf[((i*tamrbuf)/size) - 1] + rbuf[((i*tamrbuf)/size)]) / 2;
		}
		S[size-1] = 10000;
	}

	MPI_Bcast(S, size, MPI_INT, 0, MPI_COMM_WORLD);

	sbuf = (int *)malloc(size * sizeof(int));
	send_displ = (int *)malloc(size * sizeof(int));

	j = 0; k = 1;
	for (i = 0; i < tam; i++) 
	{
		if (SubVetor[i] < S[j])
			k++;
		else 
		{ 
			sbuf[j] = k-1; 
			j++; 
			k = 1; 
			i--; 
		} 
	}
	
	sbuf[j] = k-1;
	send_displ[0] = 0;
	
	for (i = 1; i < size; i++) 
	{
		send_displ[i] = send_displ[i-1] + sbuf[i-1];
	}

	MPI_Alltoall(sbuf, 1, MPI_INT, rbuf, 1, MPI_INT, MPI_COMM_WORLD);

	rec_displ = (int *)malloc(size * sizeof(int));
	rec_displ[0] = 0;
	for (i = 1; i < size; i++) 
	{
		rec_displ[i] = rec_displ[i-1] + rbuf[i-1];
	}

	BuckVetor = (int *)malloc((TAMMAX) * sizeof(int));

	MPI_Alltoallv(SubVetor, sbuf, send_displ, MPI_INT, BuckVetor, rbuf, rec_displ, MPI_INT, MPI_COMM_WORLD);

	tamB = 0;
	for (i = 0; i < size; i++) 
	{
		tamB += rbuf[i];
	}

	qsort(BuckVetor, tamB, sizeof(int), Compar);

	if (rank == 0) 
	{
		for (i = 0; i < tamB; i++) {
			printf("rank %d e BuckVetor[%d]: %d\n", rank, i, BuckVetor[i]);
		}
	}

	MPI_Finalize();
	return 0;
}
