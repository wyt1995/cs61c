#include "ex1.h"

void v_add_naive(double* x, double* y, double* z) {
    #pragma omp parallel
    {
        for (int i = 0; i < ARRAY_SIZE; i++)
            z[i] = x[i] + y[i];
    }
}

void v_add_optimized_for(double* x, double* y, double* z) {
    #pragma omp parallel for
    for (int i = 0; i < ARRAY_SIZE; i++) {
        z[i] = x[i] + y[i];
    }
}

// Adjacent Method
void v_add_optimized_adjacent(double* x, double* y, double* z) {
    // Do NOT use the `for` directive here!
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        for (int i = thread_id; i < ARRAY_SIZE; i += num_threads) {
            z[i] = x[i] + y[i];
        }
    }
}

// Chunks Method
void v_add_optimized_chunks(double* x, double* y, double* z) {
    // Do NOT use the `for` directive here!
    #pragma omp parallel
    {
        int num_threads = omp_get_num_threads();
        int thread_id = omp_get_thread_num();
        int chunk_size = ARRAY_SIZE / num_threads;
        int chunk_start = thread_id * chunk_size;

        for (int i = chunk_start; i < chunk_start + chunk_size; i++) {
            z[i] = x[i] + y[i];
        }

        for (int j = chunk_size * num_threads; j < ARRAY_SIZE; j++) {
            z[j] = x[j] + y[j];
        }
    }
}
