#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
int main()
{
    typedef ui unsigned int;
    double pi;
    double start_time, run_time;

    step = 1.0 / (double)num_steps;
    double sum_all = 0;

    start_time = omp_get_wtime();

#pragma omp parallel
    {
        double x, sum = 0;
        ui total_threads=omp_get_num_threads();
        ui chunk = num_steps/total_threads+1;

        ui id = omp_get_thread_num();

        for (int i = id*chunk+1; i <= num_steps && i<=(id+1)*chunk; i++)
        {
            x = (i - 0.5) * step;
            sum = sum + 4.0 / (1.0 + x * x);
        }
        #pragma omp critical
        sum_all += sum;
    }
    pi = step * sum_all;
    run_time = omp_get_wtime() - start_time;
    printf("\n pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
}
