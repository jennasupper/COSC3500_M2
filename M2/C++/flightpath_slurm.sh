#!/bin/bash -l
#SBATCH --job-name=flightpath
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --time=0-00:59 # time (D-HH:MM)

time ./flightpath_final