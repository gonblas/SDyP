#include <stdio.h>
#include <stdlib.h>
#include "../../utils/utils.h"

double dwalltime();

void check_result(double *matrix, int size) {
  int i, j, check = 1;
  for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			check=check&&(matrix[i*size+j]==size);
		}
	} 

  if(check){
		printf("Multiplicacion de matrices resultado correcto\n");
	}else{
		printf("Multiplicacion de matrices resultado erroneo\n");
	}
}

int main(int argc, char *argv[]) {
  double timetick, *A, *A_mod, *result;
  int i, j, k, N;
  if(argc < 2) {
    printf("Ingrese los parámetros requeridos. Uso: $s <N>\n", argv[0]);
    return 1;
  }

  A=(double*)malloc(sizeof(double)*N*N);
  result=(double*)malloc(sizeof(double)*N*N);

  for(i=0;i<N;i++){ // Por filas
		for(j=0;j<N;j++){
			A[i*N+j]=1;
		}
	}

  timetick = dwalltime();

  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      for(k=0;k<N;k++){
        result[i*N + j] += A[i*N + k] * A[k*N + j];
      }
    }
  }

  printf("Tiempo en segundos CON MATRICES ORDENADAS IGUAL %f \n", dwalltime() - timetick);

  check_result(result, N);

  A_mod = (double*)malloc(sizeof(double)*N*N);

  timetick = dwalltime(); //Se tiene en cuenta la creación de la nueva matriz

  for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			A_mod[i+j*N]=A[i*N+j];
		}
	}

  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      for(k=0;k<N;k++){
        result[i*N + j] += A[i*N + k] * A_mod[k + j*N];
      }
    }
  }

  printf("Tiempo en segundos CON MATRICES ORDENADAS DISTINTO %f \n", dwalltime() - timetick);

  check_result(result, N);

  return 0;
}
