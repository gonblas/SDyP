#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include "../../utils/utils.h"

#define SIZE(n) n*sizeof(int)

double dwalltime();
int *vec, N, T, min = INT_MAX, max = INT_MIN;
pthread_mutex_t mutex_min, mutex_max;


void *mult_matrix(void *arg) {
  int id = *(int*)arg, aux_min = INT_MAX, aux_max = INT_MIN;

  int start_index = id * (N / T);
  int end_index = (id + 1) * (N / T);
  
  if (id == T - 1) {
    end_index = N;
  }

  for(int i=start_index; i<end_index;i++) {
    if(vec[i] < aux_min) {
      aux_min = vec[i];
    }
    if(vec[i] > aux_max) {
      aux_max = vec[i];
    }
  }

  pthread_mutex_lock(&mutex_min);
  if(aux_min < min){ 
    min=aux_min;
  }
  pthread_mutex_unlock(&mutex_min);
  pthread_mutex_lock(&mutex_max);
  if(aux_max > max) {
    max=aux_max;
  }
  pthread_mutex_unlock(&mutex_max);
  pthread_exit(NULL);
}

int main(int argv, char *argc[]) {
  if(argv < 3) {
    printf("Por favor ingrese los parametros requeridos. \nUso: %s <N> <T>", argc[0]);
  }
  double timetick, sequential_time, parallel_time;
  int i, j, k;
  
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

  for(int i=0; i<N;i++) {
    if(vec[i] < min) {
      min = vec[i];
    }
    if(vec[i] > max) {
      max = vec[i];
    }
  }

  sequential_time = dwalltime() - timetick;
  printf("Tiempo secuencial con N=%d: %f \n", N, T, sequential_time);
  if(min != 0 || max != N-1) {
    printf("Error en el calculo. min = %d, max = %d \n", min, max);
  }

  min = INT_MAX; max = INT_MIN;

  // Paralelo
  pthread_t threads[T];
  pthread_mutex_init(&mutex_min, NULL);
  pthread_mutex_init(&mutex_max, NULL);

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
  if(min != 0 || max != N-1) {
    printf("Error en el calculo. min = %d, max = %d \n", min, max);
  }

  pthread_mutex_destroy(&mutex_min);
  pthread_mutex_destroy(&mutex_max);
  printf("Speedup %f\n", N, T, sequential_time/parallel_time);
  printf("Efficiency %f\n", N, T, (sequential_time/parallel_time)/T);
  
  return 0;
}