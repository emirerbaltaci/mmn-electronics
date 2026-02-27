#ifndef AUVMATH_H
#define AUVMATH_H

#include <stdint.h>
#include <stdbool.h>

void matrix_add(const float *A, const float *B, float *C, int rows, int cols);
void matrix_sub(const float *A, const float *B, float *C, int rows, int cols);
void matrix_mult(const float *A, const float *B, float *C, int rowsA, int colsA, int colsB);
void matrix_transpose(const float *A, float *AT, int rows, int cols);
bool cholesky_inverse(const float *S, float *S_inv, int n);
void quat_mult(const float *q1, const float *q2, float *q_out);
void quat_inv(const float *q, float *q_inv);
void quat_to_euler(const float *q, float *phi, float *theta, float *psi);
void quat_from_euler(float phi, float theta, float psi, float *q);
void quat_to_rot_matrix(const float *q, float *R);

void eskf_predict(float *nominal_x, float *P, const float *accel, const float *gyro, const float *Q, float dt);
void eskf_inject(float *nominal_x, float *error_x, float *P);
bool eskf_update(float *error_x, float *P, const float *dz, const float *H, const float *R_meas, int n);

#define AUVMATH_MAX_STATES 15 // 15 Error states: pos(3), vel(3), ori(3), bg(3), ba(3)
#define AUVMATH_MAX_MEAS 6

#endif // AUVMATH_H
