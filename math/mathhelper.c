/**
 * @file mathhelper.c
 * @brief Matrix and Math Helper Functions Implementation
 */

#include "mathhelper.h"
#include <string.h>
#include <math.h>

// Transpose: B = A^T
void mat_trans(const float *A, float *B, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            B[j * rows + i] = A[i * cols + j];
        }
    }
}

// Add: C = A + B
void mat_add(const float *A, const float *B, float *C, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        C[i] = A[i] + B[i];
    }
}

// Subtract: C = A - B
void mat_sub(const float *A, const float *B, float *C, int rows, int cols) {
    for (int i = 0; i < rows * cols; i++) {
        C[i] = A[i] - B[i];
    }
}

// Multiply: C = A * B
// A: [r1 x c1], B: [c1 x c2], C: [r1 x c2]
void mat_mul(const float *A, const float *B, float *C, int r1, int c1, int c2) {
    for (int i = 0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            float sum = 0.0f;
            for (int k = 0; k < c1; k++) {
                sum += A[i * c1 + k] * B[k * c2 + j];
            }
            C[i * c2 + j] = sum;
        }
    }
}

// Invert: inv = A^-1 (Gauss-Jordan)
// Returns true if successful, false if singular
bool mat_inv(const float *A, float *inv, int n) {
    float A_copy[100]; // Max 10x10 supported on stack
    if (n > 10) return false; 
    
    // Copy A to local stack buffer to allow A and inv to overlap or A to be const
    memcpy(A_copy, A, n * n * sizeof(float));
    
    // Initialize inv to Identity
    memset(inv, 0, n * n * sizeof(float));
    for (int i = 0; i < n; i++) inv[i * n + i] = 1.0f;

    for (int i = 0; i < n; i++) {
        // Find pivot
        float pivot = A_copy[i * n + i];
        int pivot_row = i;
        
        for (int k = i + 1; k < n; k++) {
            if (fabsf(A_copy[k * n + i]) > fabsf(pivot)) {
                pivot = A_copy[k * n + i];
                pivot_row = k;
            }
        }
        
        if (fabsf(pivot) < 1e-6f) return false; // Singular

        // Swap rows if needed
        if (pivot_row != i) {
            for (int k = 0; k < n; k++) {
                float tmp = A_copy[i * n + k];
                A_copy[i * n + k] = A_copy[pivot_row * n + k];
                A_copy[pivot_row * n + k] = tmp;
                
                tmp = inv[i * n + k];
                inv[i * n + k] = inv[pivot_row * n + k];
                inv[pivot_row * n + k] = tmp;
            }
        }

        // Scale row i
        float inv_pivot = 1.0f / pivot;
        for (int k = 0; k < n; k++) {
            A_copy[i * n + k] *= inv_pivot;
            inv[i * n + k] *= inv_pivot;
        }

        // Eliminate other rows
        for (int k = 0; k < n; k++) {
            if (k != i) {
                float factor = A_copy[k * n + i];
                for (int j = 0; j < n; j++) {
                    A_copy[k * n + j] -= factor * A_copy[i * n + j];
                    inv[k * n + j] -= factor * inv[i * n + j];
                }
            }
        }
    }
    return true;
}
