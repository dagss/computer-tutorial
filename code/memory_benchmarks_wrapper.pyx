cimport numpy as cnp
cimport openmp
from cython cimport parallel

cdef extern:
    double benchmark_bandwidth_c(size_t size, int repeats, int nthreads)
    
    double benchmark_latency_c(cnp.uint64_t *indices, size_t n, int repeats,
                               cnp.uint64_t *result)

def benchmark_bandwidth(size_t size, int nthreads, int repeats):
    return benchmark_bandwidth_c(size, repeats, nthreads)
                             
def benchmark_latency(cnp.ndarray[cnp.uint64_t, mode='c'] indices, int repeats):
    cdef cnp.uint64_t out
    return benchmark_latency_c(&indices[0], indices.shape[0], repeats, &out)


def get_num_threads():
    cdef int result
    cdef int *presult = &result
    with nogil:
        with parallel.parallel():
            presult[0] = openmp.omp_get_num_threads()
    return result

