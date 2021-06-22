#!/bin/bash
#SBATCH --job-name=median_filter_serial
#SBATCH --output=result_median_filter_serial.txt
#SBATCH --partition=nu
#SBATCH --ntasks=1
#SBATCH --time=00:40:00

srun ./median_filter_serial ../../video 240
