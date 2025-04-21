#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "../../utils/utils.h"

void check_result(double *matrix, int size)
{
  int i, j, check = 1;
  for (i = 0; i < size; i++)
  {
    for (j = 0; j < size; j++)
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
  }
}

void multiplyMatrix(double *A, double *B, double *result, int N, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < N; j++)
    {
      int sum = 0;
      for (int k = 0; k < N; k++)
      {
        sum += A[i * N + k] * B[k + j * N];
      }
      result[i * N + j] = sum;
    }
  }
}

void printMatrix(double *matrix, int N, int rows)
{
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < N; j++)
    {
      printf("%f ", matrix[i * N + j]);
    }
    printf("\n");
  }
  printf("\n");
}

double fSequentialProcess(double *A, double *B, double *result, int N)
{
  int i, j;
  A = (double *)malloc(sizeof(double) * N * N);
  B = (double *)malloc(sizeof(double) * N * N);
  result = (double *)malloc(sizeof(double) * N * N);
  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      A[i * N + j] = 1;
      B[i + j * N] = 1;
      result[i * N + j] = 0;
    }
  }
  double timetick = dwalltime();
  multiplyMatrix(A, B, result, N, N);
  double seq_time = dwalltime() - timetick;
  check_result(result, N);
  return seq_time;
}

int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);
  int id, processes_quantity, N, start_row, end_row, rows;
  double *A, *B, *result;
  double seq_time, timetick;
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &processes_quantity);

  N = atoi(argv[1]);

  // Confío en que el usuario introduce un N divisible por el número de procesos
  start_row = id * (N / processes_quantity);
  end_row = (id + 1) * (N / processes_quantity);
  rows = end_row - start_row;

  if (id == 0)
  {
    int i, j;
    A = (double *)malloc(sizeof(double) * N * N);
    B = (double *)malloc(sizeof(double) * N * N);
    result = (double *)malloc(sizeof(double) * N * N);
    for (i = 0; i < N; i++)
    {
      for (j = 0; j < N; j++)
      {
        A[i * N + j] = 1;
        B[i + j * N] = 1;
        result[i * N + j] = 0;
      }
    }
    timetick = dwalltime();
    multiplyMatrix(A, B, result, N, N);
    seq_time = dwalltime() - timetick;
    check_result(result, N);
    timetick = dwalltime();
  }
  else
  {
    A = (double *)malloc(sizeof(double) * N * (rows));
    B = (double *)malloc(sizeof(double) * N * N);
    result = (double *)malloc(sizeof(double) * N * (rows));
  }

  MPI_Scatter(A, N * (rows), MPI_DOUBLE, A, N * (rows), MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  multiplyMatrix(A, B, result, N, rows);

  MPI_Gather(result, N * (rows), MPI_DOUBLE, result, N * (rows), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (id == 0)
  {
    double parallel_time = dwalltime() - timetick;
    check_result(result, N);
    calculateMetrics(seq_time, parallel_time, processes_quantity);
  }
  MPI_Finalize();
  return (0);
}