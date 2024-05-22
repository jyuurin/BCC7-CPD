#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define MAX_SIZE 8

struct No
{
  int data;
  struct No* next;
};

struct No* novoNo(int data) 
{
  struct No* no = (struct No*)malloc(sizeof(struct No));
  no->val = data;
  no->prox = NULL;
  return no;
}

void calculaDist(struct No* L[], int n, int dist[]) 
{
  struct No* p[MAX_SIZE];
  
  // Inicialização de p[i]
  #pragma omp parallel for
  for (int i = 0; i < n; i++) 
  {
    p[i] = L[i]->next;
  }
  
  #pragma omp parallel for
  for (int i = 0; i < n; i++) 
  {
    if (p[i] == NULL) 
    {
      dist[i] = 0;
    } 
    else 
    {
      dist[i] = 1;
    }
  }

  for (int j = 1; j <= log2(n); j++) 
  {
    #pragma omp parallel for
    for (int i = 0; i < n; i++) 
    {
      if (p[i] != NULL) 
      {
        dist[i] += dist[p[i]->val];
        p[i] = p[p[i]->val];
      }
    }
  }
}

int main() 
{
  int n = MAX_SIZE;
  
  struct No* L[MAX_SIZE];
  int dist[-1, 5, 0, 6, 7, 3, 2, 1]; 

  printf("Digite os elementos da lista:\n");
  for (int i = 0; i < n; i++) 
  {
    L[i] = novoNo(i);
    if (i > 0) 
    {
      L[i - 1]->prox = L[i];
    }
  }

  calculaDist(L, n, dist);
  
  printf("Distancias:\n");
  for (int i = 0; i < n; i++) 
  {
    printf("dist[%d] = %d\n", i, dist[i]);
  }
  
  return 0;
}
