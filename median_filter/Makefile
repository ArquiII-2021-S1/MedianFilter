CFLAGS=-Wall -Wextra -g -std=gnu17
# LDLIBS = -lpng 
# LDLIBS = -lpng -fopenmp -fopenacc
LDLIBS = -lpng -fopenmp 
LDLIBSGPU = -lpng -fopenacc
# IMAGE_N=15
# IMAGE_M=10

PROGRAMS= median_filter median-filter-omp-cpu median-filter-omp-gpu median-filter-oacc-cpu median-filter-oacc-gpu


all: $(PROGRAMS)

test: median_filter run

median_filter: median_filter.c
	gcc median_filter.c -lpng -fopenmp -o median_filter_serial -std=c99

median-filter-omp-cpu: median-filter-omp-cpu.c
	nvc -mp=multicore median-filter-omp-cpu.c $(LDLIBS) -o median-filter-omp-cpu  -std=c99

median-filter-omp-gpu: median-filter-omp-gpu.c
	nvc median-filter-omp-gpu.c $(LDLIBS) -o median-filter-omp-gpu  -std=c99

median-filter-oacc-cpu: median-filter-oacc-cpu.c
	nvc -acc=multicore median-filter-oacc-cpu.c $(LDLIBS) -o median-filter-oacc-cpu

median-filter-oacc-gpu: median-filter-oacc-gpu.c
	nvc -acc -Minfo=accel median-filter-oacc-gpu.c $(LDLIBS) -o median-filter-oacc-gpu


clean:
	$(RM) *.o $(PROGRAMS) &> /dev/null || true

cpu-omp:
	gcc median-filter-omp-cpu.c $(LDLIBS) -o  median-filter-omp-cpu -std=c99
	sbatch media.slurm 

load:
	module load gcc/9.3.0
	module load pgi/2019

test-serial:
	gcc median_filter.c -lpng -fopenmp -o median_filter_serial -std=c99
	sbatch test-serial.slurm 
	watch -n 2 squeue -n median_filter_serial

test-omp-cpu:
	gcc median-filter-omp-cpu.c $(LDLIBS) -o median-filter-omp-cpu -std=c99
	sbatch test-omp-cpu.slurm
	watch -n 2 squeue -n median-filter-omp-cpu

test-omp-gpu:
	nvc -mp -ta=tesla,cc35 median-filter-omp-gpu.c -lpng -o median-filter-omp-gpu -std=c99
	sbatch test-omp-gpu.slurm
	watch -n 2 squeue -n median-filter-omp-gpu

test-oacc-cpu:
	pgcc median-filter-oacc-cpu.c -lpng -ta=multicore -o median-filter-oacc-cpu
	sbatch test-oacc-cpu.slurm 
	watch -n 2 squeue -n median-filter-oacc-cpu

test-oacc-gpu:
	pgcc -acc=gpu -ta=tesla,cc70 median-filter-oacc-gpu.c  -lpng -o median-filter-oacc-gpu 
	sbatch test-oacc-gpu.slurm
	watch -n 2 squeue -n median-filter-oacc-gpu

test-clean:
	rm *.txt
	rm median_filter_serial
	rm median-filter-omp-cpu
	rm median-filter-omp-gpu 
	rm median-filter-oacc-cpu
	rm median-filter-oacc-gpu


gprof-median-serial:
	gcc -pg -O0 -g median_filter.c -lpng -fopenmp -o median_filter_serial -std=c99
	./median_filter_serial ../../video 240
	gprof -a -b median_filter_serial gmon.out > analysis-serial.txt

gprof-opm-cpu:
	gcc -pg -O0 -g median-filter-omp-cpu.c $(LDLIBS) -o median-filter-omp-cpu -std=c99
	./median-filter-omp-cpu ../../video 16
	gprof -a -b median-filter-omp-cpu gmon.out > analysis-omp-cpu.txt
gprof-omp-gpu:
	gcc median-filter-omp-gpu.c $(LDLIBS) -o median-filter-omp-gpu -std=c99
	


