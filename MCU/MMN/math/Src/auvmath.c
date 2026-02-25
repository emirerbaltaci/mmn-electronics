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

    static float HT[AUVMATH_MAX_STATES * AUVMATH_MAX_MEAS];
    matrix_transpose(H, HT, meas_dim, n_states);

    static float P_HT[AUVMATH_MAX_STATES * AUVMATH_MAX_MEAS];
    matrix_mult(P, HT, P_HT, n_states, n_states, meas_dim);

    static float H_P_HT[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];
    matrix_mult(H, P_HT, H_P_HT, meas_dim, n_states, meas_dim);

    static float S[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];
    matrix_add(H_P_HT, R, S, meas_dim, meas_dim);

    static float S_inv[AUVMATH_MAX_MEAS * AUVMATH_MAX_MEAS];
    if (!cholesky_inverse(S, S_inv, meas_dim)) {
        return;
    }

    static float K[AUVMATH_MAX_STATES * AUVMATH_MAX_MEAS];
    matrix_mult(P_HT, S_inv, K, n_states, meas_dim, meas_dim);

    static float Ky[AUVMATH_MAX_STATES];
    matrix_mult(K, y, Ky, n_states, meas_dim, 1);
    matrix_add(x, Ky, x, n_states, 1);

    static float KH[AUVMATH_MAX_STATES * AUVMATH_MAX_STATES];
    matrix_mult(K, H, KH, n_states, meas_dim, n_states);

    static float I_KH[AUVMATH_MAX_STATES * AUVMATH_MAX_STATES];
    for (int i = 0; i < n_states * n_states; i++) {
        I_KH[i] = 0.0f;
    }
    for (int i = 0; i < n_states; i++) {
        I_KH[i * n_states + i] = 1.0f;
    }
    
    matrix_sub(I_KH, KH, I_KH, n_states, n_states);

    static float P_new[AUVMATH_MAX_STATES * AUVMATH_MAX_STATES];
    matrix_mult(I_KH, P, P_new, n_states, n_states, n_states);

    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < n_states; j++) {
            P[i * n_states + j] = 0.5f * (P_new[i * n_states + j] + P_new[j * n_states + i]);
        }
    }
}

void ekf_predict_12state(float *x, float *P, const float *Q, float dt) {
    float phi   = x[3];
    float theta = x[4];
    float psi   = x[5];

    float u = x[6];
    float v = x[7];
    float w = x[8];
    float p = x[9];
    float q = x[10];
    float r = x[11];

    float c_phi = cosf(phi),   s_phi = sinf(phi);
    float c_the = cosf(theta), s_the = sinf(theta), t_the = tanf(theta);
    float c_psi = cosf(psi),   s_psi = sinf(psi);

    if (fabsf(c_the) < 0.001f) c_the = (c_the < 0.0f) ? -0.001f : 0.001f;

    float x_dot = u * (c_the * c_psi) +
                  v * (s_phi * s_the * c_psi - c_phi * s_psi) +
                  w * (c_phi * s_the * c_psi + s_phi * s_psi);

    float y_dot = u * (c_the * s_psi) +
                  v * (s_phi * s_the * s_psi + c_phi * c_psi) +
                  w * (c_phi * s_the * s_psi - s_phi * c_psi);

    float z_dot = u * (-s_the) +
                  v * (s_phi * c_the) +
                  w * (c_phi * c_the);

    float phi_dot   = p + (q * s_phi + r * c_phi) * t_the;
    float theta_dot = q * c_phi - r * s_phi;
    float psi_dot   = (q * s_phi + r * c_phi) / c_the;

    x[0] += x_dot * dt;
    x[1] += y_dot * dt;
    x[2] += z_dot * dt;
    x[3] += phi_dot * dt;
    x[4] += theta_dot * dt;
    x[5] += psi_dot * dt;

    static float F[144];
    for(int i=0; i<144; i++) F[i] = 0.0f; // Sıfırla
    for(int i=0; i<12; i++) F[i*12 + i] = 1.0f; // Köşegeni 1 yap (Identity)

    F[0*12 + 3] = ( v * (c_phi * s_the * c_psi + s_phi * s_psi) + w * (-s_phi * s_the * c_psi + c_phi * s_psi) ) * dt;
    F[0*12 + 4] = ( u * (-s_the * c_psi) + v * (s_phi * c_the * c_psi) + w * (c_phi * c_the * c_psi) ) * dt;
    F[0*12 + 5] = ( u * (-c_the * s_psi) + v * (-s_phi * s_the * s_psi - c_phi * c_psi) + w * (-c_phi * s_the * s_psi + s_phi * c_psi) ) * dt;
    F[0*12 + 6] = (c_the * c_psi) * dt;
    F[0*12 + 7] = (s_phi * s_the * c_psi - c_phi * s_psi) * dt;
    F[0*12 + 8] = (c_phi * s_the * c_psi + s_phi * s_psi) * dt;

    F[1*12 + 3] = ( v * (c_phi * s_the * s_psi - s_phi * c_psi) + w * (-s_phi * s_the * s_psi - c_phi * c_psi) ) * dt;
    F[1*12 + 4] = ( u * (-s_the * s_psi) + v * (s_phi * c_the * s_psi) + w * (c_phi * c_the * s_psi) ) * dt;
    F[1*12 + 5] = ( u * (c_the * c_psi) + v * (s_phi * s_the * c_psi - c_phi * s_psi) + w * (c_phi * s_the * c_psi + s_phi * s_psi) ) * dt;
    F[1*12 + 6] = (c_the * s_psi) * dt;
    F[1*12 + 7] = (s_phi * s_the * s_psi + c_phi * c_psi) * dt;
    F[1*12 + 8] = (c_phi * s_the * s_psi - s_phi * c_psi) * dt;

    F[2*12 + 3] = ( v * (c_phi * c_the) + w * (-s_phi * c_the) ) * dt;
    F[2*12 + 4] = ( u * (-c_the) + v * (-s_phi * s_the) + w * (-c_phi * s_the) ) * dt;
    F[2*12 + 5] = 0.0f;
    F[2*12 + 6] = (-s_the) * dt;
    F[2*12 + 7] = (s_phi * c_the) * dt;
    F[2*12 + 8] = (c_phi * c_the) * dt;

    F[3*12 + 3] = ( q * c_phi - r * s_phi ) * t_the * dt;
    F[3*12 + 4] = ( q * s_phi + r * c_phi ) / (c_the * c_the) * dt;
    F[3*12 + 9]  = 1.0f * dt;
    F[3*12 + 10] = (s_phi * t_the) * dt;
    F[3*12 + 11] = (c_phi * t_the) * dt;

    F[4*12 + 3] = ( -q * s_phi - r * c_phi ) * dt;
    F[4*12 + 4] = 0.0f;
    F[4*12 + 9]  = 0.0f;
    F[4*12 + 10] = c_phi * dt;
    F[4*12 + 11] = -s_phi * dt;

    F[5*12 + 3] = ( q * c_phi - r * s_phi ) / c_the * dt;
    F[5*12 + 4] = ( (q * s_phi + r * c_phi) * s_the ) / (c_the * c_the) * dt;
    F[5*12 + 9]  = 0.0f;
    F[5*12 + 10] = (s_phi / c_the) * dt;
    F[5*12 + 11] = (c_phi / c_the) * dt;

    static float FT[144];
    static float P_FT[144];
    static float F_P_FT[144];

    matrix_transpose(F, FT, 12, 12);
    matrix_mult(P, FT, P_FT, 12, 12, 12);
    matrix_mult(F, P_FT, F_P_FT, 12, 12, 12);

    matrix_add(F_P_FT, Q, P, 12, 12);

    for (int i = 0; i < 12; i++) {
        for (int j = i + 1; j < 12; j++) {
            float avg = 0.5f * (P[i * 12 + j] + P[j * 12 + i]);
            P[i * 12 + j] = avg;
            P[j * 12 + i] = avg;
        }
    }
}
