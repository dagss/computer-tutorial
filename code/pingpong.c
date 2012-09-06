#include <omp.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "lookatme.h"

#define CACHELINESIZE 64
#define N (50 * 1000 * 1000)
#define NSAMPLES 30

int main() {

  /* Allocate a memory region *common* to all threads */
  int32_t *buffer = malloc(CACHELINESIZE * omp_get_max_threads());

#pragma omp parallel firstprivate(buffer)
  {
    int threadnum = omp_get_thread_num();
    double t_same_line = 1e10;
    for (int r = 0; r != NSAMPLES; ++r) {
      double t0 = omp_get_wtime();
      for (int i = 0; i < N; ++i) {
        buffer[threadnum] = i;
        look_at_me(buffer);
      }
      double dt = omp_get_wtime() - t0;
      if (dt < t_same_line) {
        t_same_line = dt;
      }
    }
    if (threadnum == 0) {
      printf("Time taken when sharing cache line: %.2e s\n", t_same_line);
    }

#pragma omp barrier
    double t_multi_line = 1e10;
    for (int r = 0; r != NSAMPLES; ++r) {
      double t0 = omp_get_wtime();
      for (int i = 0; i < N; ++i) {
        buffer[threadnum * (CACHELINESIZE / 4)] = i;
        look_at_me(buffer);
      }
      double dt = omp_get_wtime() - t0;
      if (dt < t_same_line) {
        t_multi_line = dt;
      }
    }
    if (threadnum == 0) {
      printf("Time taken with different cache lines: %.2e s\n", t_multi_line);
      printf("Speedup: %.2f\n", t_same_line / t_multi_line);
    }
  }

  free(buffer);

  return 0;
}
