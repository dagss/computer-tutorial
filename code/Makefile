# Normally I use 'waf' for building. Others use 'cmake'. But for the
# tutorial I use Make since that's probably familiar to more people.

CFLAGS=-g -std=c99 -Wall -fPIC -fopenmp -mtune=native

PYCFLAGS=$(shell python-config --cflags) -fPIC -fopenmp
PYLDFLAGS=$(shell python-config --ldflags) -L$(shell python-config --prefix)/lib -fpthread
PYLIBS=$(shell python-config --libs)
NUMPY_INCLUDE=-I$(shell python -c 'import numpy; print(numpy.get_include())')
PYINCLUDES=$(NUMPY_INCLUDE) $(shell python-config --includes)

binary: binary.c
	gcc $(CFLAGS) -o binary binary.c

knapsack: knapsack.c
	gcc $(CFLAGS) -o knapsack knapsack.c

memory_benchmarks: memory_benchmarks.c memory_benchmarks_wrapper.pyx
	gcc $(CFLAGS) -O3 -c -o memory_benchmarks.o memory_benchmarks.c
	objdump -M intel -S memory_benchmarks.o > memory_benchmarks.s
	cython memory_benchmarks_wrapper.pyx
	gcc $(PYCFLAGS) $(PYINCLUDES) -c -o memory_benchmarks_wrapper.o \
	  memory_benchmarks_wrapper.c
	gcc -shared $(PYLDFLAGS) -o memory_benchmarks_wrapper.so \
          memory_benchmarks_wrapper.o memory_benchmarks.o $(PYLIBS)

clean:
	rm -f *.o *.so binary knapsack

