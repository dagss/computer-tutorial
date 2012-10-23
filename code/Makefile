# Normally I use 'waf' for building. Others use 'cmake'. But for the
# tutorial I use Make since that's probably familiar to more people.

PYCFLAGS=$(shell python-config --cflags) -fPIC -fopenmp
PYLDFLAGS=$(shell python-config --ldflags) -L$(shell python-config --prefix)/lib -fpthread
PYLIBS=$(shell python-config --libs)
NUMPY_INCLUDE=-I$(shell python -c 'import numpy; print(numpy.get_include())')
PYINCLUDES=$(NUMPY_INCLUDE) $(shell python-config --includes)

add_and_square: add_and_square.c Makefile
	gcc -Wall -O1 -g -c -o add_and_square.o add_and_square.c
	gcc -o add_and_square add_and_square.o

binary: binary.c
	gcc -Wall -g -O0 -c -o binary.o binary.c
	gcc -o binary binary.o

knapsack: knapsack.c Makefile
	gcc -Wall -g -O0 -std=c99 -c -o knapsack.o knapsack.c
	gcc -o knapsack knapsack.o

memory_benchmarks: memory_benchmarks.c memory_benchmarks_wrapper.pyx
	gcc -g -std=c99 -Wall -fPIC -fopenmp -mtune=native -O3 \
          -c -o memory_benchmarks.o memory_benchmarks.c
	cython memory_benchmarks_wrapper.pyx
	gcc $(PYCFLAGS) $(PYINCLUDES) -c -o memory_benchmarks_wrapper.o \
	  memory_benchmarks_wrapper.c
	gcc -shared $(PYLDFLAGS) -o memory_benchmarks_wrapper.so \
          memory_benchmarks_wrapper.o memory_benchmarks.o $(PYLIBS) \
          -Wl,-R. -L. -llookatme

transpose: transpose.f90 Makefile
	gfortran -fopenmp -Wall -O3 -mtune=native -o transpose transpose.f90

sse_transpose: sse_transpose.c Makefile
	gcc -O3 -g -std=c99 -Wall -mtune=native -fopenmp -c -o sse_transpose.o sse_transpose.c
	gcc -fopenmp -o sse_transpose sse_transpose.c

liblookatme.so: lookatme.c Makefile
	gcc -shared -fPIC -o liblookatme.so lookatme.c

pingpong: pingpong.c liblookatme.so Makefile
	gcc -O3 -Wall -std=c99 -g -fopenmp -c -o pingpong.o pingpong.c
	gcc -fopenmp -o pingpong pingpong.o -Wl,-R. -L. -llookatme

recursion: recursion.c
	gcc -O0 -g -Wall -std=c99 -o recursion recursion.c

clean:
	rm -f *.o *.so binary knapsack

