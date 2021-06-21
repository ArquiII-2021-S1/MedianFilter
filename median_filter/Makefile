CFLAGS=-Wall -Wextra -g -std=gnu17
# LDLIBS = -lpng 
# LDLIBS = -lpng -fopenmp -fopenacc
LDLIBS = -lpng -fopenmp 
# IMAGE_N=15
# IMAGE_M=10

PROGRAMS= median_filter median-filter-omp-cpu median-filter-omp-gpu median-filter-oacc-cpu median-filter-oacc-gpu


all: $(PROGRAMS)

test: median_filter run

median_filter: median_filter.c
	gcc median_filter.c -lpng -fopenmp -o median_filter_serial -std=c99
run:
	./median_filter_serial ../../video 16

median-filter-omp-cpu: median-filter-omp-cpu.c
	nvc -mp=multicore median-filter-omp-cpu.c $(LDLIBS) -o median-filter-omp-cpu

median-filter-omp-gpu: median-filter-omp-gpu.c
	nvc -mp=gpu median-filter-omp-gpu.c $(LDLIBS) -o median-filter-omp-gpu

median-filter-oacc-cpu: median-filter-oacc-cpu.c
	nvc -acc=multicore median-filter-oacc-cpu.c $(LDLIBS) -o median-filter-oacc-cpu

median-filter-oacc-gpu: median-filter-oacc-gpu.c
	nvc -acc -Minfo=accel median-filter-oacc-gpu.c $(LDLIBS) -o median-filter-oacc-gpu


clean:
	$(RM) *.o $(PROGRAMS) &> /dev/null || true

cpu-omp:
	gcc median-filter-omp-cpu.c $(LDLIBS) -o  median-filter-omp-cpu -std=c99
	sbatch media.slurm 
