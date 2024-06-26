#include <stdio.h>
#include <math.h>
#include <omp.h>

//Avaliação de Polinômios
double avalia(int i, int j, double c[], double x) 
{
  double a, b;
  if (i == j - 1) 
  {
    return c[i] + c[j] * x;
  } 
  else 
  {
    #pragma omp parallel sections
    {
      #pragma omp section
      {
        a = avalia(i, (i + j - 1) / 2, c, x);
      }
      #pragma omp section
      {
        b = avalia((i + j - 1) / 2, j, c, x);
      }
    }
    return a + b * pow(x, (j - i) / 2);
  }
}

int main() {
  int n;
  printf("grau do polinomio (n): ");
  scanf("%d", &n);

  double c[n];
  printf("Coeficientes do polinomio:\n");
  for (int i = 0; i < n; i++) {
      printf("c[%d]: ", i);
      scanf("%lf", &c[i]);
  }

  double x;
  printf("valor de x: ");
  scanf("%lf", &x);

  // Avaliação do polinômio
  double resultado = avalia(0, n - 1, c, x);

  printf("polinomio para x = %.2lf é: %.2lf\n", x, resultado);

  return 0;
}
