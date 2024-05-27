#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_CADEIRAS 5
#define TEMPO_ENTRE_CLIENTES 5
#define TEMPO_CORTAR_CABELO 2 

sem_t clientes;
sem_t barbeiros;
sem_t mutex;
sem_t corte;
int clientes_espera = 0;

void *barbeiro_func(void *arg) 
{
	while(1) 
	{
		//dorme
		sem_wait(&clientes);
		sem_wait(&mutex);
		clientes_espera--;
		//barbeiro pronto
		sem_post(&barbeiros); 
		sem_post(&mutex);
		printf("Barbeiro trabalhando\n");
		sleep(TEMPO_CORTAR_CABELO);
		//corte feito
		sem_post(&corte);
	}
	
	return NULL;
}

void *cliente_func(void *arg) 
{
	sem_wait(&mutex);
	if (clientes_espera < NUM_CADEIRAS) 
	{
		clientes_espera++;
		//cliente esperando
		sem_post(&clientes); 
		sem_post(&mutex);
		//barbeiro ocupado com outro cliente
		sem_wait(&barbeiros); 
		//esperando o corte ser feito.
		sem_wait(&corte); 
		printf("barbeiro esta cortando o cabelo do cliente\n");
	}
	else 
	{
		sem_post(&mutex);
		printf("lugares ocupados, cliente foi embora\n");
	}
	return NULL;
}

int main() {
	sem_init(&clientes, 0, 0);
	sem_init(&barbeiros, 0, 0);
	sem_init(&mutex, 0, 1);
	sem_init(&corte, 0, 0);

	pthread_t barbeiro, cliente;
	pthread_create(&barbeiro, NULL, barbeiro_func, NULL);

	int num_clientes = 10;
	for (int i = 0; i < num_clientes; ++i) 
	{
		pthread_create(&cliente, NULL, cliente_func, NULL);
		sleep(TEMPO_ENTRE_CLIENTES);
	}

	pthread_join(barbeiro, NULL);
	sem_destroy(&clientes);
	sem_destroy(&barbeiros);
	sem_destroy(&mutex);
	sem_destroy(&corte);

	return 0;
}
