#!/bin/sh
NUM_PROC=$1
FISIN=$2
FISOUT=$3

module load libraries/openmpi-1.6-gcc-4.4.6
time mpirun ~/tema_mpi/main $FISIN $FISOUT
