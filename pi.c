#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
int main()
{
    double pi;
    double start_time, run_time;
    unsigned int n_threads = 32;

    double *sum = new double[n_threads];

    step = 1.0 / (double)num_steps;
    double sum_all = 0;

    start_time = omp_get_wtime();
    for(unsigned int i=0; i<n_threads;i++)
        sum[i] = 0;

#pragma omp parallel
    {
        n_threads = omp_get_num_threads();
        double x, sum = 0;
        unsigned int chunk = num_steps/n_threads+1;

        unsigned int id = omp_get_thread_num();

        for (int i = id*chunk+1; i <= num_steps && i<=(id+1)*chunk; i++)
        {
            x = (i - 0.5) * step;
            sum[id] = sum[id] + 4.0 / (1.0 + x * x);
        }
    }
    for(unsigned int i=0; i<n_threads;i++)
        sum_all+=sum[i];

    pi = step * sum_all;
    run_time = omp_get_wtime() - start_time;
    printf("\n pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
}
