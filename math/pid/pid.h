/**
 * @file pid.h
 * @brief Generic PID Controller Driver
 */

#ifndef PID_H
#define PID_H

typedef struct {
    float kp;           /**< Proportional Gain */
    float ki;           /**< Integral Gain */
    float kd;           /**< Derivative Gain */

    float dt;           /**< Sample time in seconds */

    float out_min;      /**< Minimum Output Limit */
    float out_max;      /**< Maximum Output Limit */
    float int_min;      /**< Minimum Integral Limit (Anti-windup) */
    float int_max;      /**< Maximum Integral Limit (Anti-windup) */

    float prev_error;   /**< Previous Error (for Derivative) */
    float integral;     /**< Accumulated Integral */
} pid_t;

/**
 * @brief Initialize PID Controller
 * 
 * @param pid Pointer to PID structure
 * @param kp Proportional Gain
 * @param ki Integral Gain
 * @param kd Derivative Gain
 * @param dt Sample time [s]
 * @param out_min Output Minimum
 * @param out_max Output Maximum
 * @param int_min Integral Minimum
 * @param int_max Integral Maximum
 */
void pid_init(pid_t *pid, float kp, float ki, float kd, float dt, 
              float out_min, float out_max, float int_min, float int_max);

/**
 * @brief Reset PID State
 * Clears integral and previous error.
 * 
 * @param pid Pointer to PID structure
 */
void pid_reset(pid_t *pid);

/**
 * @brief Update PID Controller
 * 
 * @param pid Pointer to PID structure
 * @param setpoint Desired value
 * @param measurement Current measured value
 * @return float Control output
 */
float pid_update(pid_t *pid, float setpoint, float measurement);



#endif // PID_H
