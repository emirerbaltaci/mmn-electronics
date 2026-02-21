/**
 * @file ekf.c
 * @brief Generic Extended Kalman Filter (EKF) Driver Implementation
 */

#include "ekf.h"
#include <string.h>
#include <math.h>

/* --- EKF Core Functions --- */

void ekf_init(ekf_t *ekf, int n, float *x, float *P, float *temp) {
    ekf->n = n;
    ekf->x = x;
    ekf->P = P;
    ekf->temp = temp;
}

void ekf_predict(ekf_t *ekf, const float *F, const float *Q) {
    int n = ekf->n;
    float *P = ekf->P;
    float *temp1 = ekf->temp;                // Size n*n
    float *temp2 = ekf->temp + n * n;        // Size n*n

    // P = F * P * F^T + Q
    
    // temp1 = F * P
    mat_mul(F, P, temp1, n, n, n);
    
    // temp2 = F^T
    mat_trans(F, temp2, n, n);
    
    // P = temp1 * temp2 (F*P * F^T)
    mat_mul(temp1, temp2, P, n, n, n);
    
    // P = P + Q
    mat_add(P, Q, P, n, n);
}

bool ekf_update(ekf_t *ekf, const float *z, const float *H, const float *R, const float *h_x, int m) {
    int n = ekf->n;
    float *P = ekf->P;
    float *x = ekf->x;
    
    // Memory Layout
    // temp offset 0: K [n*m] / Ht [n*m] (reused)
    // temp offset n*m: PHt [n*m] / HP [m*n] (reused, aliased)
    // temp offset 2*n*m: S [m*m] / term [n*n] (reused, aliased)
    // temp offset 2*n*m + n*n: y [m]
    
    float *K = ekf->temp;
    float *Ht = ekf->temp; // Reuse K buffer for Ht initially? 
                           // Step 2 needs Ht. Step 3 overwrites K using PHt and S_inv.
                           // K overwrites Ht. Safe.
    
    float *PHt = ekf->temp + n * m;
    float *HP = ekf->temp + n * m; // Reuse PHt buffer for HP later
    
    float *S = ekf->temp + 2 * n * m;
    float *S_inv = ekf->temp + 2 * n * m; // Alias S
    float *term = ekf->temp + 2 * n * m;  // Reuse S buffer for term (n*n >= m*m usually)
    
    float *y = ekf->temp + 2 * n * m + n * n; // Safe end buffer
    
    // 1. y = z - h(x)
    mat_sub(z, h_x, y, m, 1);
    
    // 2. S = H * P * H^T + R
    // Calculate H^T put in Ht (buffer K)
    mat_trans(H, Ht, m, n);
    
    // PHt = P * H^T (n x n * n x m -> n x m)
    mat_mul(P, Ht, PHt, n, n, m);
    
    // S = H * PHt (m x n * n x m -> m x m)
    mat_mul(H, PHt, S, m, n, m);
    
    // S = S + R
    mat_add(S, R, S, m, m);
    
    // 3. K = P * H^T * S^-1
    // Invert S -> S_inv
    if (!mat_inv(S, S_inv, m)) {
        return false;
    }
    
    // K = PHt * S_inv (n x m * m x m -> n x m)
    // Outputs to K (overwrites Ht). Inputs PHt and S_inv. Safe.
    mat_mul(PHt, S_inv, K, n, m, m);
    
    // 4. x = x + K * y
    // We need a temp vector for (K*y).
    // K [n*m], y [m]. Result [n].
    // Where to put result?
    // Can reuse PHt buffer? PHt is [n*m]. Needed? No.
    // HP aliases PHt. HP needed for Step 5.
    // So we can use PHt buffer for delta_x temporarily.
    float *delta_x = PHt; 
    mat_mul(K, y, delta_x, n, m, 1);
    
    mat_add(x, delta_x, x, n, 1);
    
    // 5. P = (I - K * H) * P
    // P = P - K * (H * P)
    
    // HP = H * P (m x n * n x n -> m x n)
    // HP alias PHt. We just used PHt for delta_x.
    // delta_x size n. PHt size n*m.
    // HP overwrites delta_x. Safe.
    mat_mul(H, P, HP, m, n, n);
    
    // term = K * HP (n x m * m x n -> n x n)
    // term aliases S_inv. S_inv not needed. Safe.
    mat_mul(K, HP, term, n, m, n);
    
    // P = P - term
    mat_sub(P, term, P, n, n);

    return true;
}
