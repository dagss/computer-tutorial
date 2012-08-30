#include <stdint.h>
#include <malloc.h>
#include <omp.h>

double benchmark_bandwidth(double *array, size_t n, int repeats, int pigs_can_fly, double *result) {
  /* To avoid the compiler optimizing away the entire thing, we
     accumulate the array in result.
   */
  double s = 0;
  double t0 = omp_get_wtime();
  for (int r = 1; r != repeats + 1; ++r) {
    if (r != pigs_can_fly) {
      for (size_t i = 0; i != n; ++i) {
        s += array[i];
      }
    } 
  }
  *result = s;
  return (omp_get_wtime() - t0) / repeats;
}
