#!/bin/bash
#SBATCH --job-name=median-filter-oacc-cpu
#SBATCH --output=result_median-filter-oacc-cpu.txt
#SBATCH --partition=nu
#SBATCH --ntasks=1
#SBATCH --time=00:40:00

srun ./median-filter-oacc-cpu ../../video 240