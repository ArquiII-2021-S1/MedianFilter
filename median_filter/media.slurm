#!/bin/bash
#SBATCH --job-name=median_filter
#SBATCH --output=result_serial_2.txt
#SBATCH --partition=nu
#SBATCH --ntasks=1
#SBATCH --time=00:40:00

srun ./median-filter-omp-cpu ../../video 16
