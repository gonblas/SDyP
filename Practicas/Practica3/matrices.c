#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../../utils/utils.h"

void verify(double *matrix, int size)
{
	int check = 1;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			check = check && (matrix[i * size + j] == size);
		}
	}

	if (check)
	{
		printf("Multiplicacion de matrices resultado correcto\n");
	}
	else
	{
		printf("Multiplicacion de matrices resultado erroneo\n");
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				printf("%f ", matrix[i * size + j]);
			}
			printf("\n");
		}
	}
}

int main(int argc, char *argv[])
{
	double *A, *B, *C;
	int i, j, k, N;
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
	B = (double *)malloc(sizeof(double) * N * N);
	C = (double *)malloc(sizeof(double) * N * N);

	// Inicializa las matrices A y B en 1, el resultado sera una matriz con todos sus valores en N
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			A[i * N + j] = 1;
			B[i + j * N] = 1;
		}
	}

	// Realiza la multiplicacion repartiendo el trabajo del primer for
	timetick = dwalltime();
	#pragma omp parallel for shared(A, B, C) private(i,j,k)
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			C[i * N + j] = 0;
			for (k = 0; k < N; k++)
			{
				C[i * N + j] += A[i * N + k] * B[k + j * N];
			}
		}
	}

	printf("Tiempo en segundos repartiendo el trabajo del primer for %f \n", dwalltime() - timetick);
	verify(C, N);
	C = (double *)malloc(sizeof(double) * N * N);

	// Realiza la multiplicacion repartiendo el trabajo del segundo for
	timetick = dwalltime();
	for (i = 0; i < N; i++)
	{
	#pragma omp parallel for shared(A, B, C, i) private(j, k)
		for (j = 0; j < N; j++)
		{
			C[i * N + j] = 0;
			for (k = 0; k < N; k++)
			{
				C[i * N + j] += A[i * N + k] * B[k + j * N];
			}
		}
	}

	printf("Tiempo en segundos repartiendo el trabajo del segundo for %f \n", dwalltime() - timetick);
	verify(C, N);

	free(A);
	free(B);
	free(C);
	return (0);
}
