#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include "../../utils/utils.h"

#define SIZE(n) n*sizeof(int)
#define TRUE 1
#define FALSE 0

double dwalltime();
int *vec, N, T;
bool isMonotonic = 1;
pthread_mutex_t mutex;

void *mult_matrix(void *arg) {
  int id = *(int*)arg, i;

  int start_index = id * (N / T);
  start_index -= (id != 0) ? 1 : 0;
  int end_index = (id + 1) * (N / T);
  int inc = TRUE, dec = TRUE;
  if (id == T - 1) {
    end_index = N;
  }

  for (int i = start_index; i < end_index; i++) {
    if (vec[i] > vec[i + 1]) {
      inc = FALSE; // No es creciente
    }
    if (vec[i] < vec[i + 1]) {
      dec = FALSE; // No es decreciente
    }
  }

  if((inc || dec) == TRUE){
    pthread_mutex_lock(&mutex);
    isMonotonic = TRUE;
    pthread_mutex_unlock(&mutex);
  }

  pthread_exit(NULL);
}

void verify(int *arr, int size) {
  if(isMonotonic != TRUE) {
    printf("Error en la ordenación: \n");
    printf("[");
    for(int i=0; i<size-1; i++) {
      printf("%d, ", vec[i]);
    }
    printf("%d]\n", vec[size-1]);
  }
}

int main(int argv, char *argc[]) {
  if(argv < 3) {
    printf("Por favor ingrese los parametros requeridos. \nUso: %s <N> <T>", argc[0]);
  }
  double timetick, sequential_time, parallel_time;
  int i;
  N = atoi(argc[1]); T = atoi(argc[2]);
  
  vec = (int*) malloc(SIZE(N));
  if (vec == NULL) {
    printf("Error: No se pudo asignar memoria para vec.\n");
    return 1;
  }

  for(i=0; i<N; i++) {
    vec[i] = i;
  }

  // Secuencial
  timetick = dwalltime();
  int inc = 1, dec = 1;
  for (int i = 0; i < N - 1; i++) {
    if (vec[i] > vec[i + 1]) {
      inc = FALSE; // No es creciente
    }
    if (vec[i] < vec[i + 1]) {
      dec = FALSE; // No es decreciente
    }
  }

  int isMonotonic = !(inc || dec);
  sequential_time = dwalltime() - timetick;
  printf("Tiempo secuencial con N=%d: %f \n", N, T, sequential_time);
  verify(vec, N);
  isMonotonic = FALSE;

  //Paralelo
  pthread_t threads[T];

  timetick = dwalltime();
  for(i=0; i<T; i++) {
    int* thread_id = malloc(sizeof(int));
    *thread_id = i;
    pthread_create(&threads[i], NULL, mult_matrix, (void*)thread_id);
  }

  for(i=0; i<T; i++) {
    pthread_join(threads[i], NULL);
  }
  parallel_time = dwalltime() - timetick;
  printf("Tiempo en paralelo con N=%d y T=%d: %f \n", N, T, parallel_time);
  verify(vec, N);

  printf("Speedup %f\n", N, T, sequential_time/parallel_time);
  printf("Efficiency %f\n", N, T, (sequential_time/parallel_time)/T);
  
  return 0;
}