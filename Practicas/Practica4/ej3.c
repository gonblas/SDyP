#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <float.h>
#include "../../utils/utils.h"

void searchDuplicates(int* vec, int* res, int start_index, int end_index, int N)
{
  for (int i = start_index; i < end_index; i++)
  {
    int j = i + 1;
    while (j < N && (vec[j] < 2 * vec[i]))
    {
      j++;
    }
    res[i - start_index] = (j < N && (vec[j] == 2 * vec[i])) ? (j - i) : (-1);
  }
}

double fSequentialProcess(int* vec, int N)
{
  int* res = malloc(sizeof(int) * N);
  double timetick = dwalltime();

  searchDuplicates(vec, res, 0, N, N);

  double time = dwalltime() - timetick;
  free(res);
  return time;
}

void fCommonProcess(int* vec, int* res_local, int N, int start_index, int end_index)
{
  MPI_Bcast(vec, N, MPI_INT, 0, MPI_COMM_WORLD);
  searchDuplicates(vec, res_local, start_index, end_index, N);
}

void fFirstProcess(int N, int start_index, int end_index, int processes_quantity)
{
  int* vec = malloc(sizeof(int) * N);
  int* res_local = malloc(sizeof(int) * (end_index - start_index));
  int* res_total = malloc(sizeof(int) * N);

  for (int i = 0; i < N; i++)
  {
    vec[i] = i;
  }

  double seq_time = fSequentialProcess(vec, N);

  double timetick = dwalltime();
  fCommonProcess(vec, res_local, N, start_index, end_index);
  MPI_Gather(res_local, end_index - start_index, MPI_INT, res_total, end_index - start_index, MPI_INT,0, MPI_COMM_WORLD);
  double parallel_time = dwalltime() - timetick;

  calculateMetrics(seq_time, parallel_time, processes_quantity);

  free(vec);
  free(res_local);
  free(res_total);
}

void fOtherProcesses(int N, int start_index, int end_index)
{
  int* vec = malloc(sizeof(int) * N);
  int* res_local = malloc(sizeof(int) * (end_index - start_index));

  fCommonProcess(vec, res_local, N, start_index, end_index);

  MPI_Gather(res_local, end_index - start_index, MPI_INT, NULL, 0, MPI_INT, 0, MPI_COMM_WORLD);

  free(vec);
  free(res_local);
}

int main(int argc, char* argv[])
{
  MPI_Init(&argc, &argv);

  int id, processes_quantity, N, start_index, end_index;
  MPI_Comm_rank(MPI_COMM_WORLD, &id);
  MPI_Comm_size(MPI_COMM_WORLD, &processes_quantity);

  if (argc < 2)
  {
    if (id == 0) printf("Uso: %s <N>\n", argv[0]);
    MPI_Finalize();
    return 1;
  }

  N = atoi(argv[1]);
  start_index = id * (N / processes_quantity);
  end_index = (id == processes_quantity - 1) ? N : (id + 1) * (N / processes_quantity);

  if (id == 0)
  {
    fFirstProcess(N, start_index, end_index, processes_quantity);
  }
  else
  {
    fOtherProcesses(N, start_index, end_index);
  }

  MPI_Finalize();
  return 0;
}
