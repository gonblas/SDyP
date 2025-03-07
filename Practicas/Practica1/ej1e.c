#include<stdio.h>
#include<stdlib.h>
#include "../../utils/utils.h"
#define ELEMENTOS_NO_CERO N*(N+1)/2
#define POS_U_FILAS(i,j) (i*N + j - i*(i+1)/2)
#define POS_L_FILAS(i,j) (j + i*(i+1)/2)

/* Time in seconds from some point in the past */
double dwalltime();

void print_matrix(double *m, int size) {
  printf("[");
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      printf("%f.1 ", m[i*size + j]);
    }
    printf("\n");
  }
  printf("]\n");
}

int main(int argc,char*argv[]){
	double *M, *L, *U, *MU,*ML,*UM,*LM;
	int i,j,k,N;
	int check=1;
	double timetick;

	//Controla los argumentos al programa
	if (argc < 2){
		printf("\n Falta un argumento:: N dimension de la matriz \n");
		return 0;
	}

	N=atoi(argv[1]);

  printf("~~~~~~~~ Almacenando ceros ~~~~~~~~\n");


	//Aloca memoria para las matrices
	M=(double*)malloc(sizeof(double)*N*N);
  L=(double*)malloc(sizeof(double)*N*N);
  U=(double*)malloc(sizeof(double)*N*N);
  MU=(double*)malloc(sizeof(double)*N*N);
  ML=(double*)malloc(sizeof(double)*N*N);
  UM=(double*)malloc(sizeof(double)*N*N);
  LM=(double*)malloc(sizeof(double)*N*N);
	
	//Inicializa las matrices M, U y L en 1, el resultado sera una matriz triangular 
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			M[i+j*N]=1;
		}
	}
  for(i=0;i<N;i++){
		for(j=i;j<N;j++){
			L[i+j*N]=1;
		}
	}
  for(i=0;i<N;i++){
		for(j=0;j<i+1;j++){
			U[i+j*N]=1;
		}
	}  

	timetick = dwalltime();
	//Realiza la multiplicacion
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			MU[i*N + j] = 0;
			for(k=0;k<j+1;k++){
        MU[i*N + j] += M[i*N + k] * U[k*N + j];
			}
			for(k=j;k<N;k++){
        ML[i*N + j] += M[i*N + k] * L[k*N + j];
			}
			for(k=i;k<N;k++){
        UM[i*N + j] += U[i*N + k] * M[k*N + j];
			}
			for(k=0;k<i+1;k++){
        LM[i*N + j] += L[i*N + k] * M[k*N + j];
			}
		}
	}
	
	printf("Tiempo en segundos %f \n", dwalltime() - timetick);

	//Verifica el resultado
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			check=check&&(MU[i*N+j]==j+1);
			check=check&&(ML[i*N+j]==N-j);
			check=check&&(UM[i*N+j]==N-i);
			check=check&&(LM[i*N+j]==i+1);
		}
	}   

	if(check){
		printf("Multiplicacion de matrices resultado correcto\n");
	}else{
		printf("Multiplicacion de matrices resultado erroneo\n");
	}

	printf("~~~~~~~~ Sin almacenar ceros ~~~~~~~~\n");

	free(U);
	free(L);
	L=(double*)malloc(sizeof(double)*ELEMENTOS_NO_CERO);
  U=(double*)malloc(sizeof(double)*ELEMENTOS_NO_CERO);

	//Inicializa las matrices resultados
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			MU[i*N+j]=0;
			ML[i*N+j]=0;
			UM[i*N+j]=0;
			LM[i*N+j]=0;
		}
	}

	for (i=0; i<ELEMENTOS_NO_CERO; i++) {
		U[i] = 1;
		L[i] = 1;
	}

	timetick = dwalltime();
	//Realiza la multiplicacion
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			MU[i*N + j] = 0;
			for(k=0;k<j+1;k++){
        MU[i*N + j] += M[i*N + k] * U[POS_U_FILAS(k,j)];
			}
			for(k=j;k<N;k++){
        ML[i*N + j] += M[i*N + k] * L[POS_L_FILAS(k,j)];
			}
			for(k=i;k<N;k++){
        UM[i*N + j] += U[POS_U_FILAS(i,k)] * M[k*N + j];
			}
			for(k=0;k<i+1;k++){
        LM[i*N + j] += L[POS_L_FILAS(i,k)] * M[k*N + j];
			}
		}
	}

	printf("Tiempo en segundos %f \n", dwalltime() - timetick);
	
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			check=check&&(MU[i*N+j]==j+1);
			check=check&&(ML[i*N+j]==N-j);
			check=check&&(UM[i*N+j]==N-i);
			check=check&&(LM[i*N+j]==i+1);
		}
	}   
	if(check){
		printf("Multiplicacion de matrices resultado correcto\n");
	}else{
		printf("Multiplicacion de matrices resultado erroneo\n");
	}

	free(M);
	free(U);
	free(L);
	free(MU);
	free(ML);
	free(LM);
	free(UM);
	return(0);
}
