#include "ex2.h"

double dotp_naive(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    for (int i = 0; i < arr_size; i++)
        global_sum += x[i] * y[i];
    return global_sum;
}

// Critical Keyword
double dotp_critical(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // Use the critical keyword here!
    #pragma omp parallel for
    for (int i = 0; i < arr_size; i++) {
        #pragma omp critical
        global_sum += x[i] * y[i];
    }
    return global_sum;
}

// Reduction Keyword
double dotp_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // Use the reduction keyword here!
    #pragma omp parallel for reduction(+:global_sum)
    for (int i = 0; i < arr_size; i++) {
        global_sum += x[i] * y[i];
    }
    return global_sum;
}

// Manual Reduction
double dotp_manual_reduction(double* x, double* y, int arr_size) {
    double global_sum = 0.0;
    // Do NOT use the `reduction` directive here!
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        int chunk_size = arr_size / num_threads;
        int chunk_start = thread_id * chunk_size;
        double local_sum = 0.0;

        for (int i = chunk_start; i < chunk_start + chunk_size; i++) {
            local_sum += x[i] * y[i];
        }

        if (thread_id == num_threads - 1) {
            for (int j = chunk_size * num_threads; j < arr_size; j++) {
                local_sum += x[j] * y[j];
            }
        }

        #pragma omp critical
        global_sum += local_sum;
    }
    return global_sum;
}
