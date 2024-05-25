#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5 // Número de filósofos
#define MAX_REFEICOES 100

#define P 0 // Pensando
#define F 1 // Com fome
#define C 2 // Comendo

int estado[N]; //estado filosofo         
sem_t sem;              
sem_t sem_filosofo[N]; //semáforos p/ cada filósofo
sem_t sem_ref; //acesso a contagem de refeições
int refeicoes[N]; //refeições de cada filósofo
int total_refeicoes = 0; //refeições totais

void exibe_mesa()
{
	for (int i = 0; i < N; i++) 
	{
		switch (estado[i]) 
		{
			case P: printf("P "); break;
			case F: printf("F "); break;
			case C: printf("C "); break;
		}
	}
	printf("\n");
}

void testa_garfos(int phil_id) 
{
	if (estado[phil_id] == F && estado[(phil_id + N - 1) % N] != F && estado[(phil_id + 1) % N] != F) 
	{
		estado[phil_id] = F;
		exibe_mesa();
		sem_post(&sem_filosofo[phil_id]);
	}
}

void pensa(int phil_id) 
{
	sleep(1);
}

void pega_garfo(int phil_id) 
{
	sem_wait(&sem);
	estado[phil_id] = F;
	exibe_mesa();
	testa_garfos(phil_id);
	sem_post(&sem);
	sem_wait(&sem_filosofo[phil_id]);
}

void come(int phil_id) 
{
	sem_wait(&sem_ref);
	refeicoes[phil_id]++;
	total_refeicoes++;
	if (total_refeicoes == MAX_REFEICOES) 
	{
		for (int i = 0; i < N; i++) 
		{
			printf("filósofo %d\n Refeições: %d\n", i, refeicoes[i]);
		}
		
		exit(0);
	}
	sem_post(&sem_ref);
	sleep(1);
}

void solta_garfo(int phil_id)
{
	sem_wait(&sem);
	estado[phil_id] = P;
	exibe_mesa();
	testa_garfos((phil_id + N - 1) % N);
	testa_garfos((phil_id + 1) % N);
	sem_post(&sem);
}

void *funcao_filosofo(void *arg)
{
	int *id = (int *)arg;
	while (1) 
	{
		pensa(*id);
		pega_garfo(*id);
		come(*id);
		solta_garfo(*id);
}
	return NULL;
}

int main() {
	pthread_t threads[N];
	int ids[N];

	sem_init(&sem, 0, 1);
	sem_init(&sem_ref, 0, 1);
	total_refeicoes = 0;
	
	for (int i = 0; i < N; i++) 
	{
		ids[i] = i;
		sem_init(&sem_filosofo[i], 0, 0);
		estado[i] = P;
		refeicoes[i] = 0;
	}

	exibe_mesa();

	for (int i = 0; i < N; i++) 
	{
		pthread_create(&threads[i], NULL, funcao_filosofo, &ids[i]);
	}

	for (int i = 0; i < N; i++) 
	{
		pthread_join(threads[i], NULL);
	}
	
	return 0;
}
