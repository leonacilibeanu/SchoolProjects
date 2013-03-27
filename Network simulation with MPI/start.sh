#!/bin/sh
NUM_PROC=$1
FIS_TOPOLOGIE=$2
FIS_MESAJE=$3

module load libraries/openmpi-1.6-gcc-4.4.6
time mpirun -np $NUM_PROC ~/tema4_apd/tema4 $FIS_TOPOLOGIE $FIS_MESAJE
