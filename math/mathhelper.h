/**
 * @file mathhelper.h
 * @brief Matrix and Math Helper Functions
 */

#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <stdint.h>
#include <stdbool.h>

/* Matrix Helper Functions */

/**
 * @brief Matrix Transpose: B = A^T
 */
void mat_trans(const float *A, float *B, int rows, int cols);

/**
 * @brief Matrix Addition: C = A + B
 */
void mat_add(const float *A, const float *B, float *C, int rows, int cols);

/**
 * @brief Matrix Subtraction: C = A - B
 */
void mat_sub(const float *A, const float *B, float *C, int rows, int cols);

/**
 * @brief Matrix Multiplication: C = A * B
 * A: [r1 x c1], B: [c1 x c2], C: [r1 x c2]
 */
void mat_mul(const float *A, const float *B, float *C, int r1, int c1, int c2);

/**
 * @brief Matrix Inversion: inv = A^-1
 * Uses Gauss-Jordan elimination.
 * @return true if successful, false if singular
 */
bool mat_inv(const float *A, float *inv, int n);



#endif // MATHHELPER_H
