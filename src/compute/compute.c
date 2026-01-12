#include <math.h>
#include "compute.h"

double run_compute(const job_packet_t *job)
{
    const int n = job->data_len;
    const double *x = job->data;

    double acc = 0.0;

    const int outer_iters = 20000;

    for (int k = 0; k < outer_iters; k++) {
        double local = 0.0;
        for (int i = 0; i < n - 1; i++) {
    
            double a = x[i];
            double b = x[i + 1];
            local += a * b + sin(a) * cos(b);
        }
        acc += local;
    }

    return acc;
}
