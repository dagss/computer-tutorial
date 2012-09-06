#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <omp.h>
#include "lookatme.h"

double benchmark_bandwidth_c(size_t n, int repeats, int nthreads) {
  double dt;
  omp_set_num_threads(nthreads);
#pragma omp parallel shared(dt)
  { 
    double *mem = malloc(n);
    memset(mem, 0, n);
#pragma omp barrier
    double t0 = omp_get_wtime();
    if (omp_get_num_threads() == nthreads) {
#pragma omp barrier
      for (int r = 0; r != repeats; ++r) {
        for (size_t i = 0; i < n / 8; i += 4) {
          mem[i] = mem[i + 1] * mem[i + 2] + mem[i + 3];
        }
        look_at_me(mem);
      }
#pragma omp barrier
      if (omp_get_thread_num() == 0) {
        dt = omp_get_wtime() - t0;
      }
    } else {
      dt = -1;
    }
    free(mem);
  }
  return dt / repeats;
}

double benchmark_latency_c(uint64_t *indices, size_t n, int repeats,
                           uint64_t *result) {
  uint64_t current_index = indices[0];
  double t0 = omp_get_wtime();
  for (int r = 0; r != repeats; ++r) {
    for (size_t i = 0; i != n; ++i) {
      current_index = indices[current_index];
    }
    *result += current_index * r;
    look_at_me(result);
  }
  return (omp_get_wtime() - t0) / repeats;
}
