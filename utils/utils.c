#include <sys/time.h>
#include<stdlib.h>
#include <stdio.h>

double dwalltime()
{
  double sec;
  struct timeval tv;

  gettimeofday(&tv,NULL);
  sec = tv.tv_sec + tv.tv_usec/1000000.0;
  return sec;
}

void calculateMetrics(double sequentialTime, double parallelTime, int numProcesses) {
  printf("Speedup: %f\n", sequentialTime / parallelTime);
  printf("Efficiency: %f\n", (sequentialTime / parallelTime) / numProcesses);
}