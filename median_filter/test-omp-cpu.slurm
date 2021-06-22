#!/bin/bash
#SBATCH --job-name=median-filter-omp-cpu
#SBATCH --output=result_median-filter-omp-cpu.txt
#SBATCH --partition=nukwa
#SBATCH --ntasks=1
#SBATCH --time=00:40:00

srun ./median-filter-omp-cpu ../../video 240