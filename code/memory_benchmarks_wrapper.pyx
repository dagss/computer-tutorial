cimport numpy as cnp

cdef extern:
    double benchmark_bandwidth_c "benchmark_bandwidth"(double *array, size_t size, int repeats,
                                                 int pigs_can_fly, double *result)
    

def benchmark_bandwidth(cnp.ndarray[double, mode='c'] array, int repeats):
    cdef double result
    return benchmark_bandwidth_c(&array[0], array.shape[0], repeats, 0, &result)
                             
                             
                           
