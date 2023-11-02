#!/bin/bash -l
#SBATCH --job-name=flightpath
#SBATCH --time=0-00:59 # time (D-HH:MM)
#SBATCH --mem=1000 # memory pool for all cores in MB (see also --mem-per-cpu)

time ./flightpath_final