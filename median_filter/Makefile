CFLAGS=-Wall -Wextra -g -std=gnu17
# LDLIBS = -lpng 
# LDLIBS = -lpng -fopenmp -fopenacc
LDLIBS = -lpng -fopenmp 
# IMAGE_N=15
# IMAGE_M=10

PROGRAMS= median_filter median-filter-omp-cpu median-filter-omp-gpu


all: $(PROGRAMS)

median_filter: median_filter.c
	nvc median_filter.c $(LDLIBS) -o median_filter

median-filter-omp-cpu: median-filter-omp-cpu.c
	nvc -mp=multicore median-filter-omp-cpu.c $(LDLIBS) -o median-filter-omp-cpu

median-filter-omp-gpu: median-filter-omp-gpu.c
	nvc -mp=gpu median-filter-omp-gpu.c $(LDLIBS) -o median-filter-omp-gpu

clean:
	$(RM) *.o $(PROGRAMS) &> /dev/null || true


