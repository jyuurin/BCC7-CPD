#include <stdio.h>
#include <omp.h>

int escolha_pivo(int A[], int m, int n) {
	int meio = (m + n) / 2;
	int pivo;

	if ((A[m] <= A[meio] && A[meio] <= A[n]) || (A[n] <= A[meio] && A[meio] <= A[m]))
		pivo = meio;
	else if ((A[m] <= A[n] && A[n] <= A[meio]) || (A[meio] <= A[n] && A[n] <= A[m]))
		pivo = n;
	else
		pivo = m;
	
	return pivo;
}

void quicksort_paralelo(int A[], int m, int n) {
	if (m < n) {
		int pivo = escolha_pivo(A, m, n);

		int lt[n - m], eq[n - m], gt[n - m];
		#pragma omp parallel for
		for (int k = 0; k <= n - m; k++) {
			lt[k] = (A[m + k] < A[pivo]);
			eq[k] = (A[m + k] == A[pivo]);
			gt[k] = (A[m + k] > A[pivo]);
		}

		// soma de prefixos
		int lt_ps[n - m + 1], eq_ps[n - m + 1], gt_ps[n - m + 1];
		int lt_cont = 0, eq_cont = 0, gt_cont = 0;
		#pragma omp parallel for
		for (int k = 0; k <= n - m; k++) {
			lt_ps[k] = lt_cont;
			eq_ps[k] = eq_cont;
			gt_ps[k] = gt_cont;
			if (lt[k]) lt_cont++;
			if (eq[k]) eq_cont++;
			if (gt[k]) gt_cont++;
		}

		int B[n - m + 1];
		#pragma omp parallel for
		for (int k = 0; k <= n - m; k++) {
			if (lt[k])
				B[lt_ps[k]] = A[m + k];
			else if (eq[k])
				B[lt_cont + eq_ps[k]] = A[m + k];
			else
				B[lt_cont + eq_cont + gt_ps[k]] = A[m + k];
		}

		#pragma omp parallel for
		for (int k = 0; k <= n - m; k++) {
			A[m + k] = B[k];
		}

		//recursão
		#pragma omp task
		quicksort_paralelo(A, m, m + lt_cont - 1);
		#pragma omp task
		quicksort_paralelo(A, m + lt_cont + eq_cont, n);
		#pragma omp taskwait
	}
}

int main() {
	//vetor conforme o slide
	int A[] = {8, 1, 4, 9, 0, 3, 5, 2, 7, 6};
	int tamanho = sizeof(A) / sizeof(A[0]);

	omp_set_num_threads(4); 

	#pragma omp parallel
	{
		#pragma omp single nowait
		quicksort_paralelo(A, 0, tamanho - 1);
	}

	printf("resultado:\n");
	for (int i = 0; i < tamanho; i++) {
		printf("%d ", A[i]);
	}
	printf("\n");

	return 0;
}
