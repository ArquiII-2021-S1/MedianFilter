#!/bin/bash
#SBATCH --job-name=median-filter-oacc-gpu
#SBATCH --output=result_median-filter-oacc-gpu.txt
#SBATCH --partition=nukwa-long
#SBATCH --ntasks=1
#SBATCH --time=00:40:00

srun ./median-filter-oacc-gpu ../../video 240