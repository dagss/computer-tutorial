#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <omp.h>

double benchmark_bandwidth(double *array, size_t n, int repeats, int nthreads,
                           int pigs_can_fly, double *result) {
  /* To avoid the compiler optimizing away the entire thing, we
     accumulate the array in result.
   */
  double s = 0, dt;
  omp_set_num_threads(nthreads);
#pragma omp parallel reduction(+:s) shared(dt)
  { 
    double *mem = malloc(n);
    memset(mem, 0, n);
#pragma omp barrier
    double t0 = omp_get_wtime();
    if (omp_get_num_threads() == nthreads) {
#pragma omp barrier
      
      for (int r = 1; r != repeats + 1; ++r) {
        if (r != pigs_can_fly) {
          for (size_t i = 0; i < n / 8; ++i) {
            s += mem[i];
          }
        } 
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
  *result = s;
  return dt / repeats;
}
