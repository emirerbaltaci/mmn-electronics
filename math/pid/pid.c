/**
 * @file pid.c
 * @brief Generic PID Controller Implementation
 */

#include "pid.h"

void pid_init(pid_t *pid, float kp, float ki, float kd, float dt, 
              float out_min, float out_max, float int_min, float int_max) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->dt = dt;
    pid->out_min = out_min;
    pid->out_max = out_max;
    pid->int_min = int_min;
    pid->int_max = int_max;
    
    pid_reset(pid);
}

void pid_reset(pid_t *pid) {
    pid->integral = 0.0f;
    pid->prev_error = 0.0f;
}

float pid_update(pid_t *pid, float setpoint, float measurement) {
    float error = setpoint - measurement;
    
    // Proportional term
    float P = pid->kp * error;
    
    // Integral term
    pid->integral += error * pid->dt;
    
    // Anti-windup for integral term
    if (pid->integral > pid->int_max) {
        pid->integral = pid->int_max;
    } else if (pid->integral < pid->int_min) {
        pid->integral = pid->int_min;
    }
    float I = pid->ki * pid->integral;
    
    // Derivative term
    float derivative = (error - pid->prev_error) / pid->dt;
    float D = pid->kd * derivative;
    
    pid->prev_error = error;
    
    // Compute total output
    float output = P + I + D;
    
    // Clamp output
    if (output > pid->out_max) {
        output = pid->out_max;
    } else if (output < pid->out_min) {
        output = pid->out_min;
    }
    
    return output;
}
