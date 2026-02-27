#include "auvmath.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

void matrix_add(const float *A, const float *B, float *C, int rows, int cols) {
  for (int i = 0; i < rows * cols; i++) {
    C[i] = A[i] + B[i];
  }
}

void matrix_sub(const float *A, const float *B, float *C, int rows, int cols) {
  for (int i = 0; i < rows * cols; i++) {
    C[i] = A[i] - B[i];
  }
}

void matrix_mult(const float *A, const float *B, float *C, int rowsA, int colsA,
                 int colsB) {
  for (int i = 0; i < rowsA; i++) {
    for (int j = 0; j < colsB; j++) {
      float sum = 0.0f;

      for (int k = 0; k < colsA; k++) {
        sum += A[i * colsA + k] * B[k * colsB + j];
      }

      C[i * colsB + j] = sum;
    }
  }
}

void matrix_transpose(const float *A, float *AT, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      AT[j * rows + i] = A[i * cols + j];
    }
  }
}

bool cholesky_inverse(const float *S, float *S_inv, int n) {
  assert(n <= AUVMATH_MAX_MEAS && "Measurement matrix size exceeds maximum!");
  if (n > AUVMATH_MAX_MEAS)
    return false;

  float S_sym[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];
  float L[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];
  float Linv[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      S_sym[i * n + j] = 0.5f * (S[i * n + j] + S[j * n + i]);
    }
  }

  for (int i = 0; i < n * n; i++) {
    L[i] = 0.0f;
    Linv[i] = 0.0f;
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= i; j++) {
      float sum = 0.0f;

      for (int k = 0; k < j; k++) {
        sum += L[i * n + k] * L[j * n + k];
      }

      if (i == j) {
        float val = S_sym[i * n + i] - sum;

        if (val <= 0.0f)
          return false;

        L[i * n + i] = sqrtf(val);
      } else {
        L[i * n + j] = (S_sym[i * n + j] - sum) / L[j * n + j];
      }
    }
  }

  for (int i = 0; i < n; i++) {
    Linv[i * n + i] = 1.0f / L[i * n + i];

    for (int j = 0; j < i; j++) {
      float sum = 0.0f;

      for (int k = j; k < i; k++) {
        sum += L[i * n + k] * Linv[k * n + j];
      }

      Linv[i * n + j] = -sum / L[i * n + i];
    }
  }

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      float sum = 0.0f;
      int k_start = (i > j) ? i : j;

      for (int k = k_start; k < n; k++) {
        sum += Linv[k * n + i] * Linv[k * n + j];
      }

      S_inv[i * n + j] = sum;
    }
  }

  return true;
}

void quat_mult(const float *q1, const float *q2, float *q_out) {
  q_out[0] = q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] - q1[3] * q2[3];
  q_out[1] = q1[0] * q2[1] + q1[1] * q2[0] + q1[2] * q2[3] - q1[3] * q2[2];
  q_out[2] = q1[0] * q2[2] - q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1];
  q_out[3] = q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0];
}

void quat_inv(const float *q, float *q_inv) {
  q_inv[0] = q[0];
  q_inv[1] = -q[1];
  q_inv[2] = -q[2];
  q_inv[3] = -q[3];
}

void quat_to_euler(const float *q, float *phi, float *theta, float *psi) {
  *phi = atan2f(2.0f * (q[0] * q[1] + q[2] * q[3]),
                1.0f - 2.0f * (q[1] * q[1] + q[2] * q[2]));
  float sinp = 2.0f * (q[0] * q[2] - q[3] * q[1]);

  if (sinp >= 1.0f)
    *theta = M_PI / 2.0f;
  else if (sinp <= -1.0f)
    *theta = -M_PI / 2.0f;
  else
    *theta = asinf(sinp);

  *psi = atan2f(2.0f * (q[0] * q[3] + q[1] * q[2]),
                1.0f - 2.0f * (q[2] * q[2] + q[3] * q[3]));
}

void quat_from_euler(float phi, float theta, float psi, float *q) {
  float cp = cosf(phi * 0.5f);
  float sp = sinf(phi * 0.5f);
  float ct = cosf(theta * 0.5f);
  float st = sinf(theta * 0.5f);
  float cy = cosf(psi * 0.5f);
  float sy = sinf(psi * 0.5f);

  q[0] = cp * ct * cy + sp * st * sy;
  q[1] = sp * ct * cy - cp * st * sy;
  q[2] = cp * st * cy + sp * ct * sy;
  q[3] = cp * ct * sy - sp * st * cy;
}

void quat_to_rot_matrix(const float *q, float *R) {
  float q00 = q[0] * q[0], q11 = q[1] * q[1], q22 = q[2] * q[2],
        q33 = q[3] * q[3];
  float q01 = q[0] * q[1], q02 = q[0] * q[2], q03 = q[0] * q[3];
  float q12 = q[1] * q[2], q13 = q[1] * q[3], q23 = q[2] * q[3];

  R[0] = q00 + q11 - q22 - q33;
  R[1] = 2.0f * (q12 - q03);
  R[2] = 2.0f * (q13 + q02);
  R[3] = 2.0f * (q12 + q03);
  R[4] = q00 - q11 + q22 - q33;
  R[5] = 2.0f * (q23 - q01);
  R[6] = 2.0f * (q13 - q02);
  R[7] = 2.0f * (q23 + q01);
  R[8] = q00 - q11 - q22 + q33;
}

void eskf_predict(float *nominal_x, float *P, const float *accel,
                  const float *gyro, const float *Q, float dt) {
  float *p = &nominal_x[0];
  float *q = &nominal_x[3];
  float *v = &nominal_x[7];
  float *bg = &nominal_x[10];
  float *ba = &nominal_x[13];
  float w_x = gyro[0] - bg[0];
  float w_y = gyro[1] - bg[1];
  float w_z = gyro[2] - bg[2];
  float a_x = accel[0] - ba[0];
  float a_y = accel[1] - ba[1];
  float a_z = accel[2] - ba[2];

  float w_norm = sqrtf(w_x * w_x + w_y * w_y + w_z * w_z);
  float dq[4] = {1.0f, 0.0f, 0.0f, 0.0f};

  if (w_norm > 1e-6f) {
    float angle = w_norm * dt;
    float c_half = cosf(angle * 0.5f);
    float s_half = sinf(angle * 0.5f) / w_norm;
    dq[0] = c_half;
    dq[1] = w_x * s_half;
    dq[2] = w_y * s_half;
    dq[3] = w_z * s_half;
  }

  float q_new[4];
  quat_mult(q, dq, q_new);
  float q_norm = sqrtf(q_new[0] * q_new[0] + q_new[1] * q_new[1] +
                       q_new[2] * q_new[2] + q_new[3] * q_new[3]);

  for (int i = 0; i < 4; i++)
    q[i] = q_new[i] / q_norm;

  float R[9];
  quat_to_rot_matrix(q, R);
  float a_earth[3];

  a_earth[0] = R[0] * a_x + R[1] * a_y + R[2] * a_z;
  a_earth[1] = R[3] * a_x + R[4] * a_y + R[5] * a_z;
  a_earth[2] = R[6] * a_x + R[7] * a_y + R[8] * a_z;
  a_earth[2] += 9.80665f;

  p[0] += v[0] * dt + 0.5f * a_earth[0] * dt * dt;
  p[1] += v[1] * dt + 0.5f * a_earth[1] * dt * dt;
  p[2] += v[2] * dt + 0.5f * a_earth[2] * dt * dt;
  v[0] += a_earth[0] * dt;
  v[1] += a_earth[1] * dt;
  v[2] += a_earth[2] * dt;

  for (int j = 0; j < 15; j++) {
    P[0 * 15 + j] += dt * P[3 * 15 + j];
    P[1 * 15 + j] += dt * P[4 * 15 + j];
    P[2 * 15 + j] += dt * P[5 * 15 + j];
  }

  float a_skew[9] = {0.0f, -a_z, a_y, a_z, 0.0f, -a_x, -a_y, a_x, 0.0f};
  float R_askew[9];
  matrix_mult(R, a_skew, R_askew, 3, 3, 3);
  float dF_vel_att[9], dF_vel_ba[9];

  for (int i = 0; i < 9; i++) {
    dF_vel_att[i] = -R_askew[i] * dt;
    dF_vel_ba[i] = -R[i] * dt;
  }

  static float P_row_back[3 * 15] __attribute__((section(".ccmram")));

  for (int j = 0; j < 15; j++) {
    P_row_back[0 * 15 + j] =
        dF_vel_att[0] * P[6 * 15 + j] + dF_vel_att[1] * P[7 * 15 + j] +
        dF_vel_att[2] * P[8 * 15 + j] + dF_vel_ba[0] * P[12 * 15 + j] +
        dF_vel_ba[1] * P[13 * 15 + j] + dF_vel_ba[2] * P[14 * 15 + j];
    P_row_back[1 * 15 + j] =
        dF_vel_att[3] * P[6 * 15 + j] + dF_vel_att[4] * P[7 * 15 + j] +
        dF_vel_att[5] * P[8 * 15 + j] + dF_vel_ba[3] * P[12 * 15 + j] +
        dF_vel_ba[4] * P[13 * 15 + j] + dF_vel_ba[5] * P[14 * 15 + j];
    P_row_back[2 * 15 + j] =
        dF_vel_att[6] * P[6 * 15 + j] + dF_vel_att[7] * P[7 * 15 + j] +
        dF_vel_att[8] * P[8 * 15 + j] + dF_vel_ba[6] * P[12 * 15 + j] +
        dF_vel_ba[7] * P[13 * 15 + j] + dF_vel_ba[8] * P[14 * 15 + j];
  }

  for (int j = 0; j < 15; j++) {
    P[3 * 15 + j] += P_row_back[0 * 15 + j];
    P[4 * 15 + j] += P_row_back[1 * 15 + j];
    P[5 * 15 + j] += P_row_back[2 * 15 + j];
  }

  float w_dt[3] = {w_x * dt, w_y * dt, w_z * dt};

  for (int j = 0; j < 15; j++) {
    float row6 =
        w_dt[2] * P[7 * 15 + j] - w_dt[1] * P[8 * 15 + j] - dt * P[9 * 15 + j];
    float row7 = -w_dt[2] * P[6 * 15 + j] + w_dt[0] * P[8 * 15 + j] -
                 dt * P[10 * 15 + j];
    float row8 =
        w_dt[1] * P[6 * 15 + j] - w_dt[0] * P[7 * 15 + j] - dt * P[11 * 15 + j];
    P[6 * 15 + j] += row6;
    P[7 * 15 + j] += row7;
    P[8 * 15 + j] += row8;
  }

  for (int i = 0; i < 15; i++) {
    P[i * 15 + 0] += dt * P[i * 15 + 3];
    P[i * 15 + 1] += dt * P[i * 15 + 4];
    P[i * 15 + 2] += dt * P[i * 15 + 5];
  }

  for (int i = 0; i < 15; i++) {
    float col6 = dF_vel_att[0] * P[i * 15 + 3] + dF_vel_att[3] * P[i * 15 + 4] +
                 dF_vel_att[6] * P[i * 15 + 5] +
                 (-w_dt[2] * P[i * 15 + 7] + w_dt[1] * P[i * 15 + 8]);
    float col7 = dF_vel_att[1] * P[i * 15 + 3] + dF_vel_att[4] * P[i * 15 + 4] +
                 dF_vel_att[7] * P[i * 15 + 5] +
                 (w_dt[2] * P[i * 15 + 6] - w_dt[0] * P[i * 15 + 8]);
    float col8 = dF_vel_att[2] * P[i * 15 + 3] + dF_vel_att[5] * P[i * 15 + 4] +
                 dF_vel_att[8] * P[i * 15 + 5] +
                 (-w_dt[1] * P[i * 15 + 6] + w_dt[0] * P[i * 15 + 7]);

    P[i * 15 + 6] += col6;
    P[i * 15 + 7] += col7;
    P[i * 15 + 8] += col8;
  }

  for (int i = 0; i < 15; i++) {
    P[i * 15 + 9] -= dt * P[i * 15 + 6];
    P[i * 15 + 10] -= dt * P[i * 15 + 7];
    P[i * 15 + 11] -= dt * P[i * 15 + 8];
  }

  for (int i = 0; i < 15; i++) {
    float col12 = dF_vel_ba[0] * P[i * 15 + 3] + dF_vel_ba[3] * P[i * 15 + 4] +
                  dF_vel_ba[6] * P[i * 15 + 5];
    float col13 = dF_vel_ba[1] * P[i * 15 + 3] + dF_vel_ba[4] * P[i * 15 + 4] +
                  dF_vel_ba[7] * P[i * 15 + 5];
    float col14 = dF_vel_ba[2] * P[i * 15 + 3] + dF_vel_ba[5] * P[i * 15 + 4] +
                  dF_vel_ba[8] * P[i * 15 + 5];
    P[i * 15 + 12] += col12;
    P[i * 15 + 13] += col13;
    P[i * 15 + 14] += col14;
  }

  for (int i = 3; i < 15; i++)
    P[i * 15 + i] += Q[i * 15 + i] * dt;

  for (int i = 0; i < 15; i++) {
    for (int j = i + 1; j < 15; j++) {
      float avg = 0.5f * (P[i * 15 + j] + P[j * 15 + i]);

      P[i * 15 + j] = avg;
      P[j * 15 + i] = avg;
    }
  }
}

void eskf_inject(float *nominal_x, float *error_x, float *P) {
  float *p = &nominal_x[0];
  float *q = &nominal_x[3];
  float *v = &nominal_x[7];
  float *bg = &nominal_x[10];
  float *ba = &nominal_x[13];

  p[0] += error_x[0];
  p[1] += error_x[1];
  p[2] += error_x[2];
  v[0] += error_x[3];
  v[1] += error_x[4];
  v[2] += error_x[5];

  float dth_norm = sqrtf(error_x[6] * error_x[6] + error_x[7] * error_x[7] +
                         error_x[8] * error_x[8]);
  float dq[4] = {1.0f, 0.0f, 0.0f, 0.0f};

  if (dth_norm > 1e-6f) {
    float c_half = cosf(dth_norm * 0.5f);
    float s_half = sinf(dth_norm * 0.5f) / dth_norm;
    dq[0] = c_half;
    dq[1] = error_x[6] * s_half;
    dq[2] = error_x[7] * s_half;
    dq[3] = error_x[8] * s_half;
  }

  float q_new[4];
  quat_mult(q, dq, q_new);
  float q_norm = sqrtf(q_new[0] * q_new[0] + q_new[1] * q_new[1] +
                       q_new[2] * q_new[2] + q_new[3] * q_new[3]);

  for (int i = 0; i < 4; i++)
    q[i] = q_new[i] / q_norm;

  bg[0] += error_x[9];
  bg[1] += error_x[10];
  bg[2] += error_x[11];
  ba[0] += error_x[12];
  ba[1] += error_x[13];
  ba[2] += error_x[14];

  float half_dth[3] = {0.5f * error_x[6], 0.5f * error_x[7], 0.5f * error_x[8]};
  static float P_temp_rows[3 * 15] __attribute__((section(".ccmram")));

  for (int j = 0; j < 15; j++) {
    P_temp_rows[0 * 15 + j] = P[6 * 15 + j] + half_dth[2] * P[7 * 15 + j] -
                              half_dth[1] * P[8 * 15 + j];
    P_temp_rows[1 * 15 + j] = P[7 * 15 + j] - half_dth[2] * P[6 * 15 + j] +
                              half_dth[0] * P[8 * 15 + j];
    P_temp_rows[2 * 15 + j] = P[8 * 15 + j] + half_dth[1] * P[6 * 15 + j] -
                              half_dth[0] * P[7 * 15 + j];
  }

  for (int j = 0; j < 15; j++) {
    P[6 * 15 + j] = P_temp_rows[0 * 15 + j];
    P[7 * 15 + j] = P_temp_rows[1 * 15 + j];
    P[8 * 15 + j] = P_temp_rows[2 * 15 + j];
  }

  static float P_temp_cols[15 * 3] __attribute__((section(".ccmram")));
  for (int i = 0; i < 15; i++) {
    P_temp_cols[i * 3 + 0] = P[i * 15 + 6] + half_dth[2] * P[i * 15 + 7] -
                             half_dth[1] * P[i * 15 + 8];
    P_temp_cols[i * 3 + 1] = P[i * 15 + 7] - half_dth[2] * P[i * 15 + 6] +
                             half_dth[0] * P[i * 15 + 8];
    P_temp_cols[i * 3 + 2] = P[i * 15 + 8] + half_dth[1] * P[i * 15 + 6] -
                             half_dth[0] * P[i * 15 + 7];
  }

  for (int i = 0; i < 15; i++) {
    P[i * 15 + 6] = P_temp_cols[i * 3 + 0];
    P[i * 15 + 7] = P_temp_cols[i * 3 + 1];
    P[i * 15 + 8] = P_temp_cols[i * 3 + 2];
  }

  for (int i = 0; i < 15; i++) {
    for (int j = i + 1; j < 15; j++) {
      float avg = 0.5f * (P[i * 15 + j] + P[j * 15 + i]);

      P[i * 15 + j] = avg;
      P[j * 15 + i] = avg;
    }
  }

  for (int i = 0; i < 15; i++)
    error_x[i] = 0.0f;
}

bool eskf_update(float *error_x, float *P, const float *dz, const float *H,
                 const float *R_meas, int n) {
  if (n > AUVMATH_MAX_MEAS || n <= 0) {
    return false;
  }

  static float HT[15 * AUVMATH_MAX_MEAS] __attribute__((section(".ccmram")));
  matrix_transpose(H, HT, n, 15);
  static float P_HT[15 * AUVMATH_MAX_MEAS] __attribute__((section(".ccmram")));
  matrix_mult(P, HT, P_HT, 15, 15, n);
  static float H_P_HT[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS]
      __attribute__((section(".ccmram")));
  matrix_mult(H, P_HT, H_P_HT, n, 15, n);
  static float S[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS]
      __attribute__((section(".ccmram")));
  matrix_add(H_P_HT, R_meas, S, n, n);
  static float S_inv[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS]
      __attribute__((section(".ccmram")));

  if (!cholesky_inverse(S, S_inv, n)) {
    return false;
  }

  static float S_inv_dz[AUVMATH_MAX_MEAS] __attribute__((section(".ccmram")));
  matrix_mult(S_inv, dz, S_inv_dz, n, n, 1);
  float mahalanobis_sq = 0.0f;

  for (int i = 0; i < n; i++) {
    mahalanobis_sq += dz[i] * S_inv_dz[i];
  }

  float chi_square_threshold = 999.0f;
  if (n == 1)
    chi_square_threshold = 6.635f;
  else if (n == 2)
    chi_square_threshold = 9.210f;
  else if (n == 3)
    chi_square_threshold = 11.345f;
  else if (n == 6)
    chi_square_threshold = 16.812f;
  if (mahalanobis_sq > chi_square_threshold) {
    return false;
  }

  static float K[15 * AUVMATH_MAX_MEAS] __attribute__((section(".ccmram")));
  matrix_mult(P_HT, S_inv, K, 15, n, n);
  static float K_dz[15] __attribute__((section(".ccmram")));
  matrix_mult(K, dz, K_dz, 15, n, 1);

  for (int i = 0; i < 15; i++) {
    error_x[i] += K_dz[i];
  }

  static float B[AUVMATH_MAX_MEAS * 15] __attribute__((section(".ccmram")));
  matrix_mult(H, P, B, n, 15, 15);

  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      float sum = 0.0f;

      for (int k = 0; k < n; k++) {
        sum += K[i * n + k] * B[k * 15 + j];
      }

      P[i * 15 + j] -= sum;
    }
  }

  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      float sum = 0.0f;

      for (int k = 0; k < n; k++) {
        sum += B[k * 15 + i] * K[j * n + k];
      }

      P[i * 15 + j] -= sum;
    }
  }

  static float temp_K_S[15 * AUVMATH_MAX_MEAS]
      __attribute__((section(".ccmram")));
  matrix_mult(K, S, temp_K_S, 15, n, n);

  for (int i = 0; i < 15; i++) {
    for (int j = 0; j < 15; j++) {
      float sum = 0.0f;

      for (int k = 0; k < n; k++) {
        sum += temp_K_S[i * n + k] * K[j * n + k];
      }

      P[i * 15 + j] += sum;
    }
  }

  for (int i = 0; i < 15; i++) {
    for (int j = i; j < 15; j++) {
      float avg = 0.5f * (P[i * 15 + j] + P[j * 15 + i]);

      P[i * 15 + j] = avg;
      P[j * 15 + i] = avg;
    }
  }

  return true;
}
