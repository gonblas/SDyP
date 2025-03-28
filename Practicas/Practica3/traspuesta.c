// traspuesta.c
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/* Time in seconds from some point in the past */
double dwalltime();


void initialize_matrix(double *matrix, int size) {
  int i,j;
  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
    {
      if (i >= j)
      {
        matrix[i * size + j] = 1.0;
      }
      else
      {
        matrix[i * size + j] = 0.0;
      }
    }
  }
}


void verify(double *matrix, int size) {
  int i, j, check = 1;
  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
    {
      if (i > j)
      {
        check = check && (matrix[i * size + j] == 0.0);
      }
      else
      {
        check = check && (matrix[i * size + j] == 1.0);
      }
    }
  }

  if (check)
  {
    printf("Resultado correcto\n");
  }
  else
  {
    printf("Resultado erroneo\n");
  }
}

int main(int argc, char *argv[])
{
  double *A;
  double temp;
  int i, j, N, tid;
  double timetick;

  // Controla los argumentos al programa
  if (argc < 3)
  {
    printf("\n Faltan argumentos:: N dimension de la matriz, T cantidad de threads \n");
    return 0;
  }
  N = atoi(argv[1]);
  int numThreads = atoi(argv[2]);
  omp_set_num_threads(numThreads);

  // Aloca memoria para las matrices
  A = (double *)malloc(sizeof(double) * N * N);

  // Inicializa la matriz con unos en el triangulo inferior y ceros en el triangulo superior.
  initialize_matrix(A, N);

  // Secuencial
  timetick = dwalltime();
  for (i = 0; i < N; i++)
  {
    for (j = i + 1; j < N; j++)
    {
      temp = A[i * N + j];
      A[i * N + j] = A[j * N + i];
      A[j * N + i] = temp;
    }
  }

  printf("Tiempo en segundos del algoritmo secuencial: %f \n", dwalltime() - timetick);
  initialize_matrix(A, N);
  timetick = dwalltime();
  #pragma omp parallel default(none) private(i, j, temp, timetick, tid) shared(A, N)
  {
    tid = omp_get_thread_num();
    timetick = dwalltime();
    #pragma omp for private(i, j, temp) nowait collapse(2)
    for (i = 0; i < N; i++)
    {
      for (j = i + 1; j < N; j++)
      {
        temp = A[i * N + j];
        A[i * N + j] = A[j * N + i];
        A[j * N + i] = temp;
      }
    }
    printf("Tiempo en segundos para el thread %d: %f \n", tid, dwalltime() - timetick);
  }
  printf("Tiempo en segundos del algoritmo paralelo: %f \n", dwalltime() - timetick);

  verify(A, N);

  free(A);
  return (0);
}
