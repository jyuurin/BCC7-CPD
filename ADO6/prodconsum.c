#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const int N_ITENS = 10;
int memo[10];

int ini = 0;
int fim = 0;

void* produtor(void *v) 
{
	for (int i = 0; i < 3 * N_ITENS; i++) 
	{
		printf("produtor = %d.\n", i);     
		fim = (fim + 1) % N_ITENS;
		memo[fim] = i;
		sleep(1);  
	}
	return NULL;
}

void* consumidor(void *v) 
{
	for (int i = 0; i < 3 * N_ITENS; i++) 
	{
		ini = (ini + 1) % N_ITENS;
		printf("consumidor = %d.\n", memo[ini]);
		sleep(1);
	}
	return NULL;
}

int main() {
	pthread_t thr_produtor, thr_consumidor;

	for (int i = 0; i < N_ITENS; i++) 
	{
		memo[i] = 0;
	}
	
	pthread_create(&thr_produtor, NULL, produtor, NULL);
	pthread_create(&thr_consumidor, NULL, consumidor, NULL);

	pthread_join(thr_produtor, NULL); 
	pthread_join(thr_consumidor, NULL);

	return 0;
}
