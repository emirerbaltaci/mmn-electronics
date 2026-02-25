#ifndef AUVMATH_H
#define AUVMATH_H

#include <stdint.h>
#include <stdbool.h>

void matrix_add(const float *A, const float *B, float *C, int rows, int cols);
void matrix_sub(const float *A, const float *B, float *C, int rows, int cols);
void matrix_mult(const float *A, const float *B, float *C, int rowsA, int colsA, int colsB);
void matrix_transpose(const float *A, float *AT, int rows, int cols);
bool cholesky_inverse(const float *S, float *S_inv, int n);
void ekf_update_dynamic(float *x, float *P, const float *z, const float *h, const float *H, const float *R, int meas_dim, int n_states);
void ekf_predict_12state(float *x, float *P, const float *Q, float dt);

#define AUVMATH_MAX_STATES 16
#define AUVMATH_MAX_MEAS 6

#endif // AUVMATH_H
