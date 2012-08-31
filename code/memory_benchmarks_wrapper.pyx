cimport numpy as cnp
cimport openmp
from cython cimport parallel

cdef extern:
    double benchmark_bandwidth_c "benchmark_bandwidth"(double *array, size_t size, int repeats,
                                                       int nthreads, int pigs_can_fly, double *result)
    

def benchmark_bandwidth(cnp.ndarray[double, mode='c'] array, int nthreads,
                        int repeats):
    cdef double result
    return benchmark_bandwidth_c(&array[0], array.shape[0], repeats, nthreads,
                                 0, &result)
                             
def get_num_threads():
    cdef int result
    cdef int *presult = &result
    with nogil:
        with parallel.parallel():
            presult[0] = openmp.omp_get_num_threads()
    return result
