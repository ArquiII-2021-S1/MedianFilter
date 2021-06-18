#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define NN 4096
#define NM 4096

double A[NN][NM];
double Anew[NN][NM];

int main(int argc, char **argv)
{
    const int n = NN;
    const int m = NM;
    const int iter_max = 1000;

    const double tol = 1.0e-6;
    double error = 1.0;

    memset(A, 0, n * m * sizeof(double));
    memset(Anew, 0, n * m * sizeof(double));

    for (int j = 0; j < n; j++)
    {
        A[j][0] = 1.0;
        Anew[j][0] = 1.0;
    }

    printf("Jacobi relaxation Calculation: %d x %d mesh\n", n, m);

    double start_time, run_time;
    start_time = omp_get_wtime();
    int iter = 0;

    int runningOnGPU = 0;

    //#pragma acc data copy(A), create(Anew)
    while (error > tol && iter < iter_max)
    {
        error = 0.0;

        //omp_set_num_threads(12);
        //#pragma omp target map(alloc:Anew[:n+2][:m+2]) map(tofrom:A[:n+2][:m+2]) map(from:runningOnGPU)
        //#pragma omp target teams distribute//data map(tofrom:A) map(to:Anew)
        //{

        //if (omp_is_initial_device() == 0)
        //    runningOnGPU = 1;

        //#pragma omp parallel for reduction(max:error) collapse(2)
#pragma omp target teams distribute map(tofrom:Anew) map(to: A) reduction(max:error) 
        for (int j = 1; j < n - 1; j++)
        {
#pragma omp parallel for reduction(max:error) 
            for (int i = 1; i < m - 1; i++)
            {
                Anew[j][i] = 0.25 * (A[j][i + 1] + A[j][i - 1] + A[j - 1][i] + A[j + 1][i]);
                error = fmax(error, fabs(Anew[j][i] - A[j][i]));
            }
        }

        //printf("%f\n", error);

//#pragma omp parallel for collapse(2)
#pragma omp target teams distribute map(from        \
                                        : A) map(to \
                                                 : Anew)
        for (int j = 1; j < n - 1; j++)
        {
#pragma omp parallel for
            for (int i = 1; i < m - 1; i++)
            {
                A[j][i] = Anew[j][i];
            }
        }
        //}
        if (iter % 100 == 0)
            printf("%5d, %0.6f\n", iter, error);

        iter++;
    }

    /* If still running on CPU, GPU must not be available */
    if (runningOnGPU)
        printf("### Able to use the GPU! ### \n");
    else
        printf("### Unable to use the GPU, using CPU! ###\n");

    run_time = omp_get_wtime() - start_time;
    printf("Tiempo: %f\n", run_time);
}
