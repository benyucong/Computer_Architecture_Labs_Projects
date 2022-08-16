#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
 */

/* Generates a random double between low and high */
double rand_double(double low, double high)
{
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high)
{
    srand(seed);
    for (int i = 0; i < result->rows; i++)
    {
        for (int j = 0; j < result->cols; j++)
        {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid. Note that the matrix is in row-major order.
 */
double get(matrix *mat, int row, int col)
{
    // Task 1.1 TODO
    int index = row * mat->cols + col;
    if (index >= (mat->cols * mat->rows))
        return -1;
    return mat->data[index];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid. Note that the matrix is in row-major order.
 */
void set(matrix *mat, int row, int col, double val)
{
    // Task 1.1 TODO
    int index = row * mat->cols + col;
    mat->data[index] = val;
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols)
{
    // Task 1.2 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Allocate space for the matrix data, initializing all entries to be 0. Return -2 if allocating memory failed.
    // 4. Set the number of rows and columns in the matrix struct according to the arguments provided.
    // 5. Set the `parent` field to NULL, since this matrix was not created from a slice.
    // 6. Set the `ref_cnt` field to 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.
    if (rows <= 0 || cols <= 0)
        return -1;
    matrix *tmp_mat = (matrix *)malloc(sizeof(matrix));
    tmp_mat->data = (double *)calloc(rows * cols, sizeof(double));
    if (tmp_mat == NULL || tmp_mat->data == NULL)
    {
        // malloc failed, do something with it
        return -2;
    }
    tmp_mat->parent = NULL;
    tmp_mat->ref_cnt = 1;
    tmp_mat->rows = rows;
    tmp_mat->cols = cols;
    *mat = tmp_mat;
    return 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice and has no existing slices,
 * or that you free `mat->parent->data` if `mat` is the last existing slice of its parent matrix and its parent
 * matrix has no other references (including itself).
 */
void deallocate_matrix(matrix *mat)
{
    // Task 1.3 TODO
    // HINTS: Follow these steps.
    // 1. If the matrix pointer `mat` is NULL, return.
    // 2. If `mat` has no parent: decrement its `ref_cnt` field by 1. If the `ref_cnt` field becomes 0, then free `mat` and its `data` field.
    // 3. Otherwise, recursively call `deallocate_matrix` on `mat`'s parent, then free `mat`.
    if (mat == NULL)
        return;
    if (mat->parent == NULL)
    {
        mat->ref_cnt -= 1;
        if (mat->ref_cnt == 0)
        {
            free(mat->data);
            free(mat);
        }
    }
    else
    {
        deallocate_matrix(mat->parent);
        free(mat);
    }
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`
 * and the reference counter for `from` should be incremented. Lastly, do not forget to set the
 * matrix's row and column values as well.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 * NOTE: Here we're allocating a matrix struct that refers to already allocated data, so
 * there is no need to allocate space for matrix data.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols)
{
    // Task 1.4 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    // 3. Set the `data` field of the new struct to be the `data` field of the `from` struct plus `offset`.
    // 4. Set the number of rows and columns in the new struct according to the arguments provided.
    // 5. Set the `parent` field of the new struct to the `from` struct pointer.
    // 6. Increment the `ref_cnt` field of the `from` struct by 1.
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    // 8. Return 0 upon success.
    if (rows <= 0 || cols <= 0)
        return -1;
    matrix *tmp_matrix = (matrix *)malloc(sizeof(matrix));
    if (tmp_matrix == NULL)
    {
        // malloc failed
        return -2;
    }
    if (offset >= from->rows * from->cols)
        return -1;
    tmp_matrix->ref_cnt = 1;
    tmp_matrix->data = from->data + offset;
    tmp_matrix->rows = rows;
    tmp_matrix->cols = cols;
    tmp_matrix->parent = from;
    from->ref_cnt += 1;
    *mat = tmp_matrix;
    return 0;
}

/*
 * Sets all entries in mat to val. Note that the matrix is in row-major order.
 * SIMD Version
 */
void fill_matrix(matrix *mat, double val)
{
    // Task 1.5 TODO
    int size = mat->cols * mat->rows;
    int i;
    __m256d data_reg = _mm256_set_pd(val, val, val, val);
    for (i = 0; i < size / 4 * 4; i += 4)
    {
        _mm256_storeu_pd(mat->data + i, data_reg);
    }
    // Tail
    for (; i < size; ++i)
    {
        mat->data[i] = val;
    }
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int abs_matrix(matrix *result, matrix *mat)
{
    // Task 1.5 TODO
    result->cols = mat->cols;
    result->rows = mat->rows;
    // result->parent = NULL;
    // result->ref_cnt = 1;
    int array_size = result->cols * result->rows;
    result->data = (double *)realloc(result->data, array_size * sizeof(double));
    __m256d zeros = _mm256_setzero_pd();
    __m256d data_reg, neg_mask, neg, res;
    int i;
    for (i = 0; i < array_size / 4 * 4; i += 4)
    {
        data_reg = _mm256_loadu_pd(mat->data + i);
        neg_mask = _mm256_cmp_pd(data_reg, zeros, 1);
        neg = _mm256_and_pd(data_reg, neg_mask);
        res = _mm256_sub_pd(data_reg, neg);
        res = _mm256_sub_pd(res, neg);
        _mm256_storeu_pd(result->data + i, res);
    }

    for (; i < array_size; ++i)
    {
        result->data[i] = mat->data[i] > 0 ? mat->data[i] : -mat->data[i];
    }
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int neg_matrix(matrix *result, matrix *mat)
{
    // Task 1.5 TODO
    return 0;
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2)
{
    // Task 1.5 TODO
    result->cols = mat1->cols;
    result->rows = mat1->rows;
    int array_size = result->cols * result->rows;
    result->data = (double *)realloc(result->data, array_size * sizeof(double));
    int i;
    for (i = 0; i < array_size / 4 * 4; i += 4)
    {
        __m256d data1 = _mm256_loadu_pd(mat1->data + i);
        __m256d data2 = _mm256_loadu_pd(mat2->data + i);
        __m256d res = _mm256_add_pd(data1, data2);
        _mm256_storeu_pd(result->data + i, res);
    }
    for (; i < array_size; ++i)
    {
        result->data[i] = mat1->data[i] + mat2->data[i];
    }
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2)
{
    // Task 1.5 TODO
    return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 * You may assume `mat1`'s number of columns is equal to `mat2`'s number of rows.
 * Note that the matrix is in row-major order.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2)
{
    // Task 1.6 TODO
    result->rows = mat1->rows;
    result->cols = mat2->cols;
    int array_size = result->rows * result->cols;
    result->data = (double *)realloc(result->data, array_size * sizeof(double));
    // Transpose mat2
    matrix *mat2_transposed = NULL;
    allocate_matrix(&mat2_transposed, mat2->cols, mat2->rows);
    for (int x = 0; x < mat2_transposed->rows; ++x)
    {
        for (int y = 0; y < mat2_transposed->cols; ++y)
        {
            // Can utilize block matrix to speed up, but not necessary
            mat2_transposed->data[y + x * mat2_transposed->cols] = mat2->data[x + y * mat2_transposed->rows];
        }
    }
    // New multiply mat1 with mat2_transposed
    for (int i = 0; i < result->rows; ++i)
    {
        for (int j = 0; j < result->cols; ++j)
        {
            __m256d tmp_sum = _mm256_setzero_pd();
            int k;
            int offset1 = i * mat1->cols;
            int offset2 = j * mat2_transposed->cols;
            double res = 0;
            for (k = 0; k < mat1->cols / 4 * 4; k += 4)
            {
                __m256d vec1 = _mm256_loadu_pd(mat1->data + offset1 + k);
                __m256d vec2 = _mm256_loadu_pd(mat2_transposed->data + offset2 + k);
                tmp_sum = _mm256_fmadd_pd(vec1, vec2, tmp_sum);
            }
            double array_tmp_res[4];
            _mm256_storeu_pd(array_tmp_res, tmp_sum);
            res = array_tmp_res[0] + array_tmp_res[1] + array_tmp_res[2] + array_tmp_res[3];
            // Tail
            for (; k < mat1->cols; ++k)
            {
                res += get(mat1, i, k) * get(mat2_transposed, j, k);
            }
            set(result, i, j, res);
        }
    }
    return 0;
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 * You may assume `mat` is a square matrix and `pow` is a non-negative integer.
 * Note that the matrix is in row-major order.
 */
int pow_matrix(matrix *result, matrix *mat, int pow)
{
    // Task 1.6 TODO
    result->cols = mat->cols;
    result->rows = mat->rows;
    int array_size = result->cols * result->rows;
    result->data = (double *)realloc(result->data, array_size * sizeof(double));
    for (int i = 0; i < result->rows; ++i)
    {
        for (int j = 0; j < result->cols; ++j)
        {
            if (i == j)
                set(result, i, j, 1);
            else
                set(result, i, j, 0);
        }
    }
    if (pow)
    {
        for (int i = 0; i < pow; ++i)
        {
            matrix *tmp = NULL;
            allocate_matrix(&tmp, result->rows, result->cols);
            mul_matrix(tmp, result, mat);
            for (int j = 0; j < array_size; ++j)
            {
                result->data[j] = tmp->data[j];
            }
            deallocate_matrix(tmp);
        }
    }
    return 0;
}
