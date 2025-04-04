#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../utils/utils.h"

#define SIZE(n) n*sizeof(int)

double dwalltime();
int *vec, N, T;
double average;
pthread_mutex_t mutex;


void *mult_matrix(void *arg) {
  int id = *(int*)arg, sum = 0;

  int start_index = id * (N / T);
  int end_index = (id + 1) * (N / T);
  
  if (id == T - 1) {
    end_index = N;
  }

  for(int i=start_index; i<end_index;i++) {
    sum+=vec[i];
  }
  pthread_mutex_lock(&mutex);
  average+=(double) sum/N;
  pthread_mutex_unlock(&mutex);
  pthread_exit(NULL);
}

int main(int argv, char *argc[]) {
  if(argv < 3) {
    printf("Por favor ingrese los parametros requeridos. \nUso: %s <N> <T>", argc[0]);
  }
  double timetick, sequential_time, parallel_time;
  int i, j, k, sum = 0;
  
  N = atoi(argc[1]); T = atoi(argc[2]);
  
  vec = (int*) malloc(SIZE(N));
  if (vec == NULL) {
    printf("Error: No se pudo asignar memoria para vec.\n");
    return 1;
  }

  for(i=0; i<N; i++) {
    vec[i] = 1;
  }

  // Secuencial
  timetick = dwalltime();

  for(int i=0; i<N;i++) {
    sum+=vec[i];
  }

  average = sum/N;
  sequential_time = dwalltime() - timetick;
  printf("Tiempo secuencial con N=%d: %f \n", N, T, sequential_time);
  if(average != 1) {
    printf("Error en el calculo: %f\n", average);
  }

  average = 0;

  // Paralelo
  pthread_t threads[T];
  pthread_mutex_init(&mutex, NULL);

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
  if(average != 1) {
    printf("Error en el calculo: %f\n", average);
  }

  pthread_mutex_destroy(&mutex);
  printf("Speedup %f\n", N, T, sequential_time/parallel_time);
  printf("Efficiency %f\n", N, T, (sequential_time/parallel_time)/T);
  
  return 0;
}