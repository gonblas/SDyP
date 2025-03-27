#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../../utils/utils.h"

#define SIZE(n) n*n*sizeof(int)

double dwalltime();
int *A, *B, *C, N, T;


void *mult_matrix(void *arg) {
  int id = *(int*)arg;

  int start_row = id * (N / T);
  int end_row = (id + 1) * (N / T);
  
  if (id == T - 1) {
    end_row = N;
  }

  for(int i=start_row; i<end_row;i++) {
    for(int j=0; j<N; j++) {
      int sum = 0;
      for(int k=0; k<N; k++) {
        sum += A[i*N + k] * B[k + j*N];
      }
      C[i*N + j] = sum;
    }
  }

  pthread_exit(NULL);
}


void verify(int *C, int size) {
  int i;
  for(i = 0; i < size*size; i++){
    if(C[i] != size) {
      printf("Resultado incorrecto.");
      break;
    }
  }
}

int main(int argv, char *argc[]) {
  if(argv < 3) {
    printf("Por favor ingrese los parametros requeridos. \nUso: %s <N> <T>", argc[0]);
  }
  double timetick, sequential_time, parallel_time;
  int i, j, k;
  
  N = atoi(argc[1]); T = atoi(argc[2]);
  
  A = (int*) malloc(SIZE(N));
  B = (int*) malloc(SIZE(N));
  C = (int*) malloc(SIZE(N));

  for(i=0; i<N*N; i++) {
    A[i] = 1; B[i] = 1;
  }

  // Secuencial
  timetick = dwalltime();

  for(int i=0; i<N;i++) {
    for(j=0; j<N; j++) {
      int sum = 0;
      for(k=0; k<N; k++) {
        sum += A[i*N + k] * B[k + j*N];
      }
      C[i*N + j] = sum;
    }
  }
  sequential_time = dwalltime() - timetick;
  printf("Tiempo secuencial con N=%d: %f \n", N, T, sequential_time);
  verify(C, N);


  for(i=0; i<N*N; i++) {
    C[i] = 0;
  }

  // Paralelo
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

  verify(C, N);
  free(A); free(B); free(C);
  printf("Speedup %f\n", N, T, sequential_time/parallel_time);
  printf("Efficiency %f\n", N, T, (sequential_time/parallel_time)/T);

  return 0;
}