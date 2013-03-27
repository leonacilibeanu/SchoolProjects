#!/bin/sh

qsub -S /bin/bash -q ibm-quad.q start.sh 8 ~/tema_mpi/test/mandelbrot1.in mandelbrot1.pgm
