CFLAGS=-Wall -Wextra -g -std=gnu17
# LDLIBS = -lpng 
# LDLIBS = -lpng -fopenmp -fopenacc
LDLIBS = -lpng -fopenmp 
# IMAGE_N=15
# IMAGE_M=10

PROGRAMS= median-filter median-filter-omp-cpu median-filter-omp-gpu median-filter-oacc-cpu median-filter-oacc-gpu


all: $(PROGRAMS)

median-filter: median-filter.c
	nvc median-filter.c $(LDLIBS) -o median-filter
	./median-filter ../video 240

median-filter-omp-cpu: median-filter-omp-cpu.c
	nvc -mp=multicore -g median-filter-omp-cpu.c $(LDLIBS) -o median-filter-omp-cpu
	./median-filter-omp-cpu ../video 240

median-filter-omp-gpu: median-filter-omp-gpu.c
	nvc -mp=gpu median-filter-omp-gpu.c $(LDLIBS) -o median-filter-omp-gpu
	./median-filter-omp-gpu ../video 240

median-filter-oacc-cpu: median-filter-oacc-cpu.c
	nvc -acc=multicore median-filter-oacc-cpu.c $(LDLIBS) -o median-filter-oacc-cpu
	./median-filter-oacc-cpu ../video 240

median-filter-oacc-gpu: median-filter-oacc-gpu.c
	nvc -acc median-filter-oacc-gpu.c $(LDLIBS) -o median-filter-oacc-gpu
	./median-filter-oacc-gpu ../video 240

clean:
	$(RM) *.o $(PROGRAMS) &> /dev/null || true


