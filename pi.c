#include <stdio.h>
#include <omp.h>
static long num_steps = 100000000;
double step;
int main()
{
    double pi;
    double start_time, run_time;

    step = 1.0 / (double)num_steps;
    double sum_all = 0;

    start_time = omp_get_wtime();

#pragma omp parallel
    {
        unsigned int n_threads = omp_get_num_threads();
        double x, sum = 0;
        unsigned int chunk = num_steps / n_threads + 1;

        unsigned int id = omp_get_thread_num();

        for (int i = id; i < num_steps; i += n_threads)
        {
            x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }

#pragma omp critical
        sum_all += sum;
    }

    pi = step * sum_all;
    run_time = omp_get_wtime() - start_time;
    printf("\n pi with %ld steps is %lf in %lf seconds\n ", num_steps, pi, run_time);
}
