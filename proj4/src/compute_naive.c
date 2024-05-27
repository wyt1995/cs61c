#include "compute.h"

void flip_matrix(matrix_t *src_matrix) {
    uint32_t num_rows = src_matrix->rows;
    uint32_t num_cols = src_matrix->cols;

    // flip horizontally
    for (uint32_t i = 0; i < num_rows / 2; i++) {
        for (uint32_t j = 0; j < num_cols; j++) {
            uint32_t up = num_cols * i + j;
            uint32_t down = num_cols * (num_rows - 1 - i) + j;
            int32_t temp = src_matrix->data[up];
            src_matrix->data[up] = src_matrix->data[down];
            src_matrix->data[down] = temp;
        }
    }

    // flip vertically
    for (uint32_t i = 0; i < num_rows; i++) {
        for (uint32_t j = 0; j < num_cols / 2; j++) {
            uint32_t left = num_cols * i + j;
            uint32_t right = num_cols * i + (num_cols - 1 - j);
            int32_t temp = src_matrix->data[left];
            src_matrix->data[left] = src_matrix->data[right];
            src_matrix->data[right] = temp;
        }
    }
}

int dot_product(matrix_t *a_matrix, matrix_t *b_matrix, uint32_t index) {
    int32_t result = 0;
    for (uint32_t i = 0; i < b_matrix->rows; i++) {
        for (uint32_t j = 0; j < b_matrix->cols; j++) {
            uint32_t a_index = index + a_matrix->cols * i + j;
            uint32_t b_index = b_matrix->cols * i + j;
            result += a_matrix->data[a_index] * b_matrix->data[b_index];
        }
    }
    return result;
}

// Computes the convolution of two matrices
int convolve(matrix_t *a_matrix, matrix_t *b_matrix, matrix_t **output_matrix) {
    if (!a_matrix || !b_matrix || !output_matrix) {
        return -1;
    }
    uint32_t a_rows = a_matrix->rows;
    uint32_t a_cols = a_matrix->cols;
    uint32_t b_rows = b_matrix->rows;
    uint32_t b_cols = b_matrix->cols;
    int32_t *a_arr = a_matrix->data;
    int32_t *b_arr = b_matrix->data;
    if (!a_rows || !b_rows || !a_cols || !b_cols || !a_arr || !b_arr) {
        return -1;
    }

    // allocate memory for the resulting matrix
    matrix_t *result = (matrix_t *) malloc(sizeof(matrix_t));
    result->rows = a_rows - b_rows + 1;
    result->cols = a_cols - b_cols + 1;
    result->data = (int32_t *) calloc(result->rows * result->cols, sizeof(int32_t));

    // convolve matrix a and matrix b, and store the resulting matrix in output_matrix
    flip_matrix(b_matrix);
    for (uint32_t i = 0; i < result->rows; i++) {
        for (uint32_t j = 0; j < result->cols; j++) {
            uint32_t index = result->cols * i + j;
            result->data[index] = dot_product(a_matrix, b_matrix, index + i * (b_cols - 1));
        }
    }

    *output_matrix = result;
    return 0;
}

// Executes a task
int execute_task(task_t *task) {
    matrix_t *a_matrix, *b_matrix, *output_matrix;

    char *a_matrix_path = get_a_matrix_path(task);
    if (read_matrix(a_matrix_path, &a_matrix)) {
        printf("Error reading matrix from %s\n", a_matrix_path);
        return -1;
    }
    free(a_matrix_path);

    char *b_matrix_path = get_b_matrix_path(task);
    if (read_matrix(b_matrix_path, &b_matrix)) {
        printf("Error reading matrix from %s\n", b_matrix_path);
        return -1;
    }
    free(b_matrix_path);

    if (convolve(a_matrix, b_matrix, &output_matrix)) {
        printf("convolve returned a non-zero integer\n");
        return -1;
    }

    char *output_matrix_path = get_output_matrix_path(task);
    if (write_matrix(output_matrix_path, output_matrix)) {
        printf("Error writing matrix to %s\n", output_matrix_path);
        return -1;
    }
    free(output_matrix_path);

    free(a_matrix->data);
    free(b_matrix->data);
    free(output_matrix->data);
    free(a_matrix);
    free(b_matrix);
    free(output_matrix);
    return 0;
}
