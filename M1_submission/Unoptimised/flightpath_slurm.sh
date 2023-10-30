#!/bin/bash
#SBATCH --job-name=flightpath
#SBATCH --partition=cosc
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=1
#SBATCH --cpus-per-task=1
#SBATCH --mem-per-cpu=1G # memory (MB)
#SBATCH --time=0-00:59 # time (D-HH:MM)

time ./flightpath_draft