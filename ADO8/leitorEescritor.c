#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int N_LEITORES = 10;
int N_ESCRITORES = 3;
int bd = -1;      
int nleitores = 0;  
sem_t mutex;        
sem_t acesso;       

void *leitor(void *arg) 
{
	int id = *(int *)arg;

	sem_wait(&mutex);
	nleitores++;
	if (nleitores == 1)
		sem_wait(&acesso);
	sem_post(&mutex);

	printf("leitor %d - valor %d\n", id, bd);
	sleep(1); 
	sem_wait(&mutex);
	
	nleitores--;
	
	if (nleitores == 0)
			sem_post(&acesso);
	sem_post(&mutex);

	return NULL;
}

void *escritor(void *arg) {
	int id = *(int *)arg;
	sem_wait(&acesso);
	bd = id;
	
	printf("escritor %d - valor %d\n", id, bd);
	sleep(1); 
	
	sem_post(&acesso);

	return NULL;
}

void *gera_leitores(void *v) 
{
	pthread_t thr_leitores[N_LEITORES];
	int ids_leitores[N_LEITORES];
	int i;

	//threads leitoras - cria
	for (i = 0; i < N_LEITORES; i++) 
	{
		ids_leitores[i] = i + 1;
		pthread_create(&thr_leitores[i], NULL, leitor, &ids_leitores[i]);
	}

	//threads leitoras - termina
	for (i = 0; i < N_LEITORES; i++)
	{
		pthread_join(thr_leitores[i], NULL);
	}

	return NULL;
}

void *gera_escritores(void *v) 
{
	pthread_t thr_escritores[N_ESCRITORES];
	int ids_escritores[N_ESCRITORES];
	int i;

	//threads leitoras - cria
	for (i = 0; i < N_ESCRITORES; i++) 
	{
		ids_escritores[i] = i + 1;
		pthread_create(&thr_escritores[i], NULL, escritor, &ids_escritores[i]);
	}

	//threads leitoras - termina
	for (i = 0; i < N_ESCRITORES; i++) 
	{
		pthread_join(thr_escritores[i], NULL);
	}

	return NULL;
}

int main() {
	sem_init(&mutex, 0, 1);
	sem_init(&acesso, 0, 1);
	
	pthread_t thr_gera_esc, thr_gera_leit;
	pthread_create(&thr_gera_leit, NULL, gera_leitores, NULL);
	pthread_create(&thr_gera_esc, NULL, gera_escritores, NULL);
	pthread_join(thr_gera_leit, NULL);
	pthread_join(thr_gera_esc, NULL);

	return 0;
}
