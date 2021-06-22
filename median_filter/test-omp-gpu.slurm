#!/bin/bash
#SBATCH --job-name=median-filter-omp-gpu
#SBATCH --output=result_median-filter-omp-gpu.txt
#SBATCH --partition=nukwa
#SBATCH --ntasks=1
#SBATCH --time=00:40:00

srun ./median-filter-omp-gpu ../../video 240