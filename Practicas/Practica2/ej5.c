#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include "../../utils/utils.h"

#define SIZE(n) n*sizeof(int)

double dwalltime();
int *vec, N, T;
typedef struct {
  int left, right;
} Segment;

void merge(int *arr, int left, int mid, int right) {
  int i, j, k;
  int left_size = mid - left+1;
  int right_size = right - mid;

  int left_arr[left_size], right_arr[right_size];

  for (i = 0; i < left_size; i++)
    left_arr[i] = arr[left + i];
  for (j = 0; j < right_size; j++)
    right_arr[j] = arr[mid + 1 + j];

  i = 0; j = 0; k = left;
  while (i < left_size && j < right_size) {
      if (left_arr[i] <= right_arr[j]) {
          arr[k] = left_arr[i];
          i++;
      }
      else {
          arr[k] = right_arr[j];
          j++;
      }
      k++;
  }

  while (i < left_size) {
      arr[k] = left_arr[i];
      i++;
      k++;
  }

  while (j < right_size) {
      arr[k] = right_arr[j];
      j++;
      k++;
  }
}

void sequential_merge_sort(int *arr, int left, int right) {
  if(left >= right) {
    return;
  }

  int mid = left + (right - left)/2;

  sequential_merge_sort(arr, left, mid);
  sequential_merge_sort(arr, mid+1, right);

  merge(arr, left, mid, right);
}


void *parallel_merge_sort(void *arg) {
  int id = *((int*)arg);
  int segment_size = N / T;
  int left = id * segment_size;
  int right = (id + 1) * segment_size - 1;

  if(id == T -1) {
    right = N - 1; 
  }

  sequential_merge_sort(vec, left, right);
  pthread_exit(NULL);
}


void verify(int *arr, int size) {
  int i, j;
  for(i=0; i<size-1; i++) {
    if(vec[i] > vec[i+1]) {
      printf("Error en la ordenación: \n");
      printf("[");
      for(j=0; j<size-1; j++) {
        printf("%d, ", vec[j]);
      }
      printf("%d]\n", vec[size-1]);
      break;
    }
  }
}

int main(int argv, char *argc[]) {
  if(argv < 3) {
    printf("Por favor ingrese los parámetros requeridos. \nUso: %s <N> <T>", argc[0]);
  }
  double timetick, sequential_time, parallel_time;
  int i, j;
  
  N = atoi(argc[1]); T = atoi(argc[2]);
  
  vec = (int*) malloc(SIZE(N)); 
  if (vec == NULL) {
    printf("Error: No se pudo asignar memoria para vec.\n");
    return 1;
  }

  for(i=0; i<N; i++) {
    vec[i] = N-(i+1);
  }

  // Secuencial
  timetick = dwalltime();

  sequential_merge_sort(vec, 0, N-1);
  sequential_time = dwalltime() - timetick;
  printf("Tiempo secuencial con N=%d: %f \n", N, T, sequential_time);
  verify(vec, N);

  for(i=0; i<N; i++) {
    vec[i] = N-(i+1);
  }

  // Paralelo
  pthread_t threads[T];
  timetick = dwalltime();

  for(i=0;i<T;i++) {
    int* thread_id = malloc(sizeof(int));
    *thread_id = i;
    pthread_create(&threads[i], NULL, parallel_merge_sort, (void*)thread_id);
  }

  for(i=0;i<T;i++) {
    pthread_join(threads[i], NULL);
  }

  int segment_size = N / T;
  for (int size = segment_size; size < N; size *= 2) {
    for (int left = 0; left < N - size; left += 2 * size) {
      int mid = left + size - 1;
      int right = (left + 2 * size - 1 < N) ? (left + 2 * size - 1) : (N - 1);
      merge(vec, left, mid, right);
    }
  }

  parallel_time = dwalltime() - timetick;
  printf("Tiempo en paralelo con N=%d y T=%d: %f \n", N, T, parallel_time);
  verify(vec, N);
  free(vec);
  printf("Speedup %f\n", N, T, sequential_time/parallel_time);
  printf("Efficiency %f\n", N, T, (sequential_time/parallel_time)/T);

  return 0;
}