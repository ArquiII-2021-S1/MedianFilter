libs = -lpng -fopenmp
all: median-filter median-filter-omp-cpu median-filter-omp-gpu
.PHONY: all

median-filter: median-filter.c
	nvc median-filter.c -o median-filter $(libs)

median-filter-omp-cpu: median-filter-omp-cpu.c
	nvc -mp:multicore median-filter-omp-cpu.c -o median-filter-omp-cpu $(libs)

median-filter-omp-gpu: median-filter-omp-gpu.c
	nvc -mp:gpu median-filter-omp-gpu.c -o median-filter-omp-gpu $(libs)
