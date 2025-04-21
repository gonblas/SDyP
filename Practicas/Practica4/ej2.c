#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <float.h>
#include "../../utils/utils.h"

double fSequentialProcess(double *A, double *B, int N)
{
  int i, j;
  double min = DBL_MAX, max = DBL_MIN, avg = 0;
  double timetick = dwalltime();

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      if (A[i * N + j] < min)
      {
        min = A[i * N + j];
      }

      if (A[i * N + j] > max)
      {
        max = A[i * N + j];
      }
      avg += A[i * N + j];
    }
  }

  avg /= (N * N);
  printf("Min: %f, Max: %f, Avg: %f\n", min, max, avg);

  for (i = 0; i < N; i++)
  {
    for (j = 0; j < N; j++)
    {
      int index = i * N + j;
      if (A[index] < avg)
      {
        B[index] = min;
      }
      else if (A[index] > avg)
      {
        B[index] = max;
      }
      else
      {
        B[index] = avg;
      }
    }
  }

  return dwalltime() - timetick;
}

void fCommonProcess(int id, double *A, double *B, int N, int rows)
{
  int i, j;
  double min = DBL_MAX, max = DBL_MIN, avg = 0;

  MPI_Scatter(A, N * (rows), MPI_DOUBLE, A, N * (rows), MPI_DOUBLE, 0, MPI_COMM_WORLD);

  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < N; j++)
    {
      int index = i * N + j;
      if (A[index] < min)
      {
        min = A[index];
      }

      if (A[index] > max)
      {
        max = A[index];
      }
      avg += A[index];
    }
  }

  avg /= (N * N);

  double global_min, global_max, global_avg;
  MPI_Allreduce(&min, &global_min, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
  MPI_Allreduce(&max, &global_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
  MPI_Allreduce(&avg, &global_avg, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < N; j++)
    {
      int index = i * N + j;
      if (A[index] < global_avg)
      {
        B[index] = global_min;
      }
      else if (A[index] > global_avg)
      {
        B[index] = global_max;
      }
      else
      {
        B[index] = global_avg;
      }
    }
  }
  MPI_Gather(B, N * (rows), MPI_DOUBLE, B, N * (rows), MPI_DOUBLE, 0, MPI_COMM_WORLD);
}

void fFirstProcess(int N, int rows, int processes_quantity)
{
  double *A, *B;
  A = (double *)malloc(sizeof(double) * N * (N));
  B = (double *)malloc(sizeof(double) * N * (N));

  for (int i = 0; i < N; i++)
  {
    for (int j = 0; j < N; j++)
    {
      A[i * N + j] = i + j;
    }
  }

  double seq_time = fSequentialProcess(A, B, N);

  double timetick = dwalltime();
  fCommonProcess(0, A, B, N, rows);
  double parallel_time = dwalltime() - timetick;
  calculateMetrics(seq_time, parallel_time, processes_quantity);
}

void fOtherProcesses(int N, int rows)
{
  double *A, *B;
  A = (double *)malloc(sizeof(double) * N * rows);
  B = (double *)malloc(sizeof(double) * N * rows);

  fCommonProcess(1, A, B, N, rows);
}


int main(int argc, char *argv[])
{
  MPI_Init(&argc, &argv);
  int id, processes_quantity, N, start_row, end_row, rows;
  // double seq_time, timetick;
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &processes_quantity);

  N = atoi(argv[1]);

  // Confío en que el usuario introduce un N divisible por el número de procesos
  start_row = id * (N / processes_quantity);
  end_row = (id + 1) * (N / processes_quantity);
  rows = end_row - start_row;

  if (id == 0)
  {
    fFirstProcess(N, rows, processes_quantity);
  } else {
    fOtherProcesses(N, rows);
  }

  MPI_Finalize();
  return (0);
}
