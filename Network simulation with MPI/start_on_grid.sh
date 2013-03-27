#!/bin/sh

qsub -S /bin/bash -q ibm-nehalem.q start.sh 12 ~/tema4_apd/topologie.txt ~/tema4_apd/mesaje.txt
