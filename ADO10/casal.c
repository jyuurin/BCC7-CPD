#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_HOMENS 3
#define NUM_MULHERES 3

pthread_t homens[NUM_HOMENS];
pthread_t mulheres[NUM_MULHERES];

pthread_mutex_t mutex_homem[NUM_HOMENS];
pthread_mutex_t mutex_mulher[NUM_MULHERES];
pthread_mutex_t mutex_casais;

int homem_proposicao[NUM_HOMENS] = {0};
int mulher_proposicao[NUM_MULHERES] = {0};
int casais[NUM_HOMENS] = {-1, -1, -1};

void* homem(void* arg) {
	int id = *((int*)arg);
	while (1) 
	{
		pthread_mutex_lock(&mutex_homem[id]);
		if (homem_proposicao[id] < NUM_MULHERES) 
		{
			int mulher_id = homem_proposicao[id]++;
			pthread_mutex_lock(&mutex_mulher[mulher_id]);
			if (casais[mulher_id] == -1) {
						casais[mulher_id] = id;
						printf("Homem %d e Mulher %d tornam-se casal.\n", id, mulher_id);
			} 
			else if (mulher_id < casais[mulher_id]) 
			{
				int antigo_par = casais[mulher_id];
				casais[mulher_id] = id;
				printf("Quebrando o par (%d, %d) e formando (%d, %d).\n", antigo_par, mulher_id, id, mulher_id);
				pthread_mutex_unlock(&mutex_homem[antigo_par]);
			}
			else 
				pthread_mutex_unlock(&mutex_mulher[mulher_id]);
		} 
		else 
		{
			pthread_mutex_unlock(&mutex_homem[id]);
			break;
		}
	}
	pthread_exit(NULL);
}

void* mulher(void* arg) 
{
	int id = *((int*)arg);
	
	while (1) 
	{
			pthread_mutex_lock(&mutex_mulher[id]);
			int preferido = -1;
			for (int i = 0; i < NUM_HOMENS; ++i) 
			{
				if (mulher_proposicao[id] == 0 || (casais[id] == -1 && i > casais[id]))
				{
					preferido = i;
					break;
				}
			}
			if (preferido != -1) 
			{
				if (casais[id] == -1)
				{
					casais[id] = preferido;
					printf("Homem %d e Mulher %d tornam-se casal.\n", preferido, id);
				} 
				else 
				{
					int antigo_par = casais[id];
					casais[id] = preferido;
					printf("Quebrando o par (%d, %d) e formando (%d, %d).\n", antigo_par, id, preferido, id);
					pthread_mutex_unlock(&mutex_homem[antigo_par]);
				}
				mulher_proposicao[id] = 1;
			} 
			else 
			{
				pthread_mutex_unlock(&mutex_mulher[id]);
				break;
			}
	}
	pthread_exit(NULL);
}

int main() {
	pthread_mutex_init(&mutex_casais, NULL);
	for (int i = 0; i < NUM_HOMENS; ++i) 
	{
		pthread_mutex_init(&mutex_homem[i], NULL);
		pthread_mutex_lock(&mutex_homem[i]);
	}
	for (int i = 0; i < NUM_MULHERES; ++i) 
	{
		pthread_mutex_init(&mutex_mulher[i], NULL);
		pthread_mutex_lock(&mutex_mulher[i]);
	}
	for (int i = 0; i < NUM_HOMENS; ++i) 
	{
		int* id = (int*)malloc(sizeof(int));
		*id = i;
		pthread_create(&homens[i], NULL, homem, (void*)id);
	}
	for (int i = 0; i < NUM_MULHERES; ++i) 
	{
		int* id = (int*)malloc(sizeof(int));
		*id = i;
		pthread_create(&mulheres[i], NULL, mulher, (void*)id);
	}
	for (int i = 0; i < NUM_HOMENS; ++i)
	{
		pthread_mutex_unlock(&mutex_homem[i]);
	}
	for (int i = 0; i < NUM_HOMENS; ++i) 
	{
		pthread_join(homens[i], NULL);
	}
	for (int i = 0; i < NUM_MULHERES; ++i) 
	{
		pthread_join(mulheres[i], NULL);
	}
	for (int i = 0; i < NUM_HOMENS; ++i) 
	{
		pthread_mutex_destroy(&mutex_homem[i]);
	}
	for (int i = 0; i < NUM_MULHERES; ++i)
	{
		pthread_mutex_destroy(&mutex_mulher[i]);
	}
	
	pthread_mutex_destroy(&mutex_casais);
	
	return 0;
}
