/**
 * @file ekf.h
 * @brief Generic Extended Kalman Filter (EKF) Driver
 *
 * This driver implements a standard EKF algorithm with basic matrix operations.
 * It is designed to be platform-independent and does not rely on dynamic memory allocation.
 */

#ifndef EKF_H
#define EKF_H

#include <stdint.h>
#include <stdbool.h>
#include "../mathhelper.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief EKF Structure
 */
typedef struct {
    int n;          /**< Number of states */
    float *x;       /**< State vector [n] */
    float *P;       /**< Prediction covariance matrix [n * n] */
    float *temp;    /**< Temporary working buffer required for matrix operations */
                    /* Minimum size: 2*n*n + m (for predict) or 2*n*m + n*n + m (for update) */
                    /* Recommended: 3 * n * n + n (if m <= n) */
} ekf_t;

/**
 * @brief Initialize the EKF structure
 *
 * @param ekf Pointer to EKF structure
 * @param n Number of states
 * @param x Pointer to state vector array [n]
 * @param P Pointer to covariance matrix array [n * n]
 * @param temp Pointer to working buffer
 */
void ekf_init(ekf_t *ekf, int n, float *x, float *P, float *temp);

/**
 * @brief EKF Prediction Step
 *
 * x = f(x, u) <-- User must update state x before calling this, or provide F linearization around current x
 * P = F * P * F^T + Q
 *
 * @param ekf Pointer to EKF structure
 * @param F Jacobian of state transition matrix [n * n]
 * @param Q Process noise covariance matrix [n * n]
 */
void ekf_predict(ekf_t *ekf, const float *F, const float *Q);

/**
 * @brief EKF Update Step
 *
 * K = P * H^T * (H * P * H^T + R)^-1
 * x = x + K * (z - h(x))
 * P = (I - K * H) * P
 *
 * @param ekf Pointer to EKF structure
 * @param z Measurement vector [m]
 * @param H Jacobian of observation matrix [m * n]
 * @param R Measurement noise covariance matrix [m * m]
 * @param h_x Predicted measurement (h(x)) [m] - Innovation y = z - h_x is calculated internally
 * @param m Number of measurements
 * @return true if update successful (inversion valid), false otherwise
 */
bool ekf_update(ekf_t *ekf, const float *z, const float *H, const float *R, const float *h_x, int m);

#ifdef __cplusplus
}
#endif

#endif // EKF_H
