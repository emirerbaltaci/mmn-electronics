#include "auvmath.h"
#include <math.h>

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

void matrix_mult(const float *A, const float *B, float *C, int rowsA, int colsA, int colsB) {
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
    if (n > AUVMATH_MAX_MEAS) return false;
    float L[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];
    float Linv[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];
    
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
                float val = S[i * n + i] - sum;
                if (val <= 0.0f) return false;
                L[i * n + i] = sqrtf(val);
            } else {
                L[i * n + j] = (S[i * n + j] - sum) / L[j * n + j];
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

void ekf_update_dynamic(float *x, float *P, const float *z, const float *h, const float *H, const float *R, int meas_dim, int n_states) {
    if (n_states > AUVMATH_MAX_STATES || meas_dim > AUVMATH_MAX_MEAS) return;

    float y[AUVMATH_MAX_MEAS];
    matrix_sub(z, h, y, meas_dim, 1);

    float HT[AUVMATH_MAX_STATES * AUVMATH_MAX_MEAS];
    matrix_transpose(H, HT, meas_dim, n_states);

    float P_HT[AUVMATH_MAX_STATES * AUVMATH_MAX_MEAS];
    matrix_mult(P, HT, P_HT, n_states, n_states, meas_dim);

    float H_P_HT[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];
    matrix_mult(H, P_HT, H_P_HT, meas_dim, n_states, meas_dim);

    float S[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];
    matrix_add(H_P_HT, R, S, meas_dim, meas_dim);

    float S_inv[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];
    if (!cholesky_inverse(S, S_inv, meas_dim)) {
        return;
    }

    float K[AUVMATH_MAX_STATES * AUVMATH_MAX_MEAS];
    matrix_mult(P_HT, S_inv, K, n_states, meas_dim, meas_dim);

    float Ky[AUVMATH_MAX_STATES];
    matrix_mult(K, y, Ky, n_states, meas_dim, 1);
    matrix_add(x, Ky, x, n_states, 1);

    float KH[AUVMATH_MAX_STATES * AUVMATH_MAX_STATES];
    matrix_mult(K, H, KH, n_states, meas_dim, n_states);

    float I_KH[AUVMATH_MAX_STATES * AUVMATH_MAX_STATES];
    for (int i = 0; i < n_states * n_states; i++) {
        I_KH[i] = 0.0f;
    }
    for (int i = 0; i < n_states; i++) {
        I_KH[i * n_states + i] = 1.0f;
    }
    
    matrix_sub(I_KH, KH, I_KH, n_states, n_states);

    float P_new[AUVMATH_MAX_STATES * AUVMATH_MAX_STATES];
    matrix_mult(I_KH, P, P_new, n_states, n_states, n_states);

    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < n_states; j++) {
            P[i * n_states + j] = 0.5f * (P_new[i * n_states + j] + P_new[j * n_states + i]);
        }
    }
}
