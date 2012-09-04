Tutorial on basics of modern computers
======================================

This repository contains material for a tutorial:

http://www.mn.uio.no/astro/forskning/aktuelt/arrangementer/gjesteforelesninger-seminarer/fredagskollokvium/2012/seljebotn.html


Code inventory
--------------

The ``code`` directory contains the demonstration code used during
the tutorial. In addition to the list below, see the ``Makefile``.

binary.c:
   C program that demonstrates how scalars are encoded
   in the memory of the computer.

knapsack.c:
   Solves the Knapsack problem, in order to demonstrate the
   the stack and the heap. Use ``knapsack_problem_1.txt`` for example input.


transpose.f90:
   A program that shows off the tiling technique.

memory_benchmarks.c:
   Benchmarks the  memory bus bandwidth and latency. Python is used to set
   up the benchmark suite and plot the results.

Useful commands
---------------

Cache size:
    ``sudo dmidecode -t cache``

Show assembly code:
    ``objdump -M intel -S yourfile.o``
