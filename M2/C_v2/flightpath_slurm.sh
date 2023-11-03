#!/bin/bash -l
#SBATCH --job-name=flightpath
#SBATCH --nodes=4
#SBATCH --ntasks=4
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=4
#SBATCH --time=0-00:59 # time (D-HH:MM)
#SBATCH --partition=cosc
#SBATCH --mem=1GB # memory pool for all cores in MB (see also --mem-per-cpu)
module add mpi/openmpi-x86_64

export PATH=/opt/local/stow/cuda-11.1/bin:$PATH
export PATH=/usr/lib64/openmpi/bin:$PATH
#module load mpi

make

time mpirun -n 4 ./flightpath_final