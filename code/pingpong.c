#include <omp.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "lookatme.h"

#define CACHELINESIZE 64
#define N (30 * 1000 * 1000)

int main() {

  /* Allocate a memory region *common* to all threads */
  int8_t *buffer = malloc(CACHELINESIZE * omp_get_max_threads());

#pragma omp parallel firstprivate(buffer)
  {
    int threadnum = omp_get_thread_num();

#pragma omp barrier
    double t0 = omp_get_wtime();
    for (int i = 0; i < N; ++i) {
      buffer[threadnum] = i;
      look_at_me(buffer);
    }
#pragma omp barrier
    double t_same_line = omp_get_wtime() - t0;
    if (threadnum == 0) {
      printf("Time taken when sharing cache line: %.2e s\n", t_same_line);
    }

#pragma omp barrier
    t0 = omp_get_wtime();
    for (int i = 0; i < N; ++i) {
      buffer[threadnum * CACHELINESIZE] = i;
      look_at_me(buffer);
    }
    double t_multi_line = omp_get_wtime() - t0;
#pragma omp barrier
    if (threadnum == 0) {
      printf("Time taken with different cache lines: %.2e s\n", t_multi_line);
      printf("Speedup: %.2f\n", t_same_line / t_multi_line);
    }
  }

  free(buffer);

  return 0;
}
