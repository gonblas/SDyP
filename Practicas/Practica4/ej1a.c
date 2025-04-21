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
  for(int i=0; i<rows;i++) {
    for(int j=0; j<N; j++) {
      int sum = 0;
      for(int k=0; k<N; k++) {
        sum += A[i*N + k] * B[k + j*N];
      }
      result[i*N + j] = sum;
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

void fSenderProcess(int N, int processes_quantity)
{
  int i, j;
  double *A, *B, *result;
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

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      result[i * N + j] = 0;
    }
  }

  timetick = dwalltime();

  for (int i = 1; i < processes_quantity; i++)
  {
    int start_row = i * (N / processes_quantity);
    int end_row = (i + 1) * (N / processes_quantity);
    if (i == processes_quantity - 1)
      end_row = N;
    MPI_Send(&A[N * start_row], (end_row - start_row) * N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
    MPI_Send(B, N * N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
  }
  int start_row = i * (N / processes_quantity);
  int end_row = (i + 1) * (N / processes_quantity);

  multiplyMatrix(A, B, result, N, end_row - start_row);
  
  for(int i=1; i<processes_quantity; i++){
    int start_row = i * (N / processes_quantity);
    int end_row = (i + 1) * (N / processes_quantity);
    if(i == processes_quantity - 1) end_row = N;
    MPI_Recv(&result[N*start_row], (end_row - start_row)*N, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  double parallel_time = dwalltime() - timetick;
  check_result(result, N);
  calculateMetrics(seq_time, parallel_time, processes_quantity);
}

void fReceiverProcess(int id, int N, int processes_quantity)
{
  double *A, *B, *result;

  int start_row = id * (N / processes_quantity);
  int end_row = (id + 1) * (N / processes_quantity);
  if(id == processes_quantity - 1) end_row = N;
  int rows = end_row - start_row;
  A = (double*)malloc(sizeof(double)*N*(rows));
  B = (double*)malloc(sizeof(double)*N*N);
  result = (double*)malloc(sizeof(double)*N*(rows));
  MPI_Recv(A, N*(rows), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  MPI_Recv(B, N*N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  for(int i=0;i<N*(rows);i++){
    result[i]=0;
  }

  multiplyMatrix(A, B, result, N, rows);

  MPI_Send(result, N*(rows), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
}

int main(int argc, char *argv[])
{

  MPI_Init(&argc, &argv);
  int id, processes_quantity, N;
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &processes_quantity);

  N = atoi(argv[1]);
  if (id == 0)
    fSenderProcess(N, processes_quantity);
  else
    fReceiverProcess(id, N, processes_quantity);
  MPI_Finalize();
  return (0);
}