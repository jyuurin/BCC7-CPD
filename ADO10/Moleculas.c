#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define RODADAS 100
#define N 30

sem_t mutex;
sem_t filaOxi;
sem_t filaHidro;
sem_t barreira;

int oxigenio = 0;
int hidrogenio = 0;
int ligacoes = 0;

void formarLigacao() 
{
	printf("Formou H20!\n");
	ligacoes++;
}

void* threadOxigenio(void* arg) 
{
	sem_wait(&mutex);
	oxigenio++;
	if (hidrogenio >= 2) 
	{
		sem_post(&filaHidro);
		sem_post(&filaHidro);
		hidrogenio -= 2;
		sem_post(&filaOxi);
		oxigenio--;
	} 
	else
		sem_post(&mutex);
	
	sem_wait(&filaOxi);
	formarLigacao();
	sem_post(&barreira);
	
	return NULL;
}

void* threadHidrogenio(void* arg) 
{
	sem_wait(&mutex);
	hidrogenio++;
	if (hidrogenio >= 2 && oxigenio >= 1) 
	{
		sem_post(&filaHidro);
		sem_post(&filaHidro);
		hidrogenio -= 2;
		sem_post(&filaOxi);
		oxigenio--;
	} 
	else
		sem_post(&mutex);
	
	sem_wait(&filaHidro);
	sem_wait(&barreira);
	
	return NULL;
}

int main() {
	sem_init(&mutex, 0, 1);
	sem_init(&filaOxi, 0, 0);
	sem_init(&filaHidro, 0, 0);
	sem_init(&barreira, 0, 0);

	for (int k = 0; k < RODADAS; ++k) 
	{
		pthread_t threads[N];
		for (int i = 0; i < N; ++i) 
		{
			if (i % 3 == 0)
				pthread_create(&threads[i], NULL, threadOxigenio, NULL); 
			else 
				pthread_create(&threads[i], NULL, threadHidrogenio, NULL);
				
		}
		for (int i = 0; i < N; ++i) 
		{
			pthread_join(threads[i], NULL);
		}
		printf("%d H2O\n", ligacoes);
	}

	sem_destroy(&mutex);
	sem_destroy(&filaOxi);
	sem_destroy(&filaHidro);
	sem_destroy(&barreira);

	return 0;
}
