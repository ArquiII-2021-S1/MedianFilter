CFLAGS=-Wall -Wextra -g -std=gnu17
LDLIBS = -lpng 
# LDLIBS = -lpng -fopenmp -fopenacc
# IMAGE_N=15
# IMAGE_M=10

PROGRAMS= median_filter




all: $(PROGRAMS)

median_filter: median_filter.c
	$(CC) $(LDFLAGS) median_filter.c $(LDLIBS) -o median_filter

clean:
	$(RM) *.o $(PROGRAMS) &> /dev/null || true


