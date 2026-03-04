/*
 * MIT License
 *
 * Copyright (c) 2026 MM Nautronics
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "auvcontrol.h"
#include "thruster_config.h"
#include <math.h>

const float T_pinv[8][6] = AUV_TAM_MATRIX;

void PID_Init(PID_Controller_t *pid, float p, float i, float d, float max,
              float min, float wrap_bound) {
  pid->kp = p;
  pid->ki = i;
  pid->kd = d;
  pid->integral = 0.0f;
  pid->prev_error = 0.0f;
  pid->prev_measurement = 0.0f;
  pid->limit_max = max;
  pid->limit_min = min;
  pid->wrap_bound = wrap_bound;
  pid->first_run = true;

  if (i > 0.0f) {
    // Deliberately scales the integral limit by ki to bound the raw integral
    // scope directly, rather than globally capping the combined P/D term
    // contributions.
    pid->integral_max = max / i;
    pid->integral_min = min / i;
  } else {
    pid->integral_max = max;
    pid->integral_min = min;
  }
}

void PID_Reset(PID_Controller_t *pid) {
  pid->integral = 0.0f;
  pid->prev_error = 0.0f;
  pid->prev_measurement = 0.0f;
  pid->first_run = true;
}

float PID_Update(PID_Controller_t *pid, float setpoint, float measurement,
                 float dt) {
  if (dt <= 1e-6f)
    return 0.0f;

  if (pid->first_run) {
    pid->prev_measurement = measurement;
    pid->first_run = false;
  }

  float error = setpoint - measurement;
  if (pid->wrap_bound > 0.0f) {
    float period = 2.0f * pid->wrap_bound;
    error = fmodf(error + pid->wrap_bound, period);
    if (error < 0.0f)
      error += period;
    error -= pid->wrap_bound;
  }

  float meas_diff = measurement - pid->prev_measurement;
  if (pid->wrap_bound > 0.0f) {
    float period = 2.0f * pid->wrap_bound;
    meas_diff = fmodf(meas_diff + pid->wrap_bound, period);
    if (meas_diff < 0.0f)
      meas_diff += period;
    meas_diff -= pid->wrap_bound;
  }
  float derivative = -meas_diff / dt;

  float p_term = pid->kp * error;
  float d_term = pid->kd * derivative;

  // Conditional Integration (Anti-Windup)
  float pre_output = p_term + (pid->ki * pid->integral) + d_term;
  if ((pre_output >= pid->limit_max && error > 0.0f) ||
      (pre_output <= pid->limit_min && error < 0.0f)) {
    // Stop integrating if output is saturated and error is pushing it further
  } else {
    pid->integral += error * dt;
    if (pid->integral > pid->integral_max)
      pid->integral = pid->integral_max;
    else if (pid->integral < pid->integral_min)
      pid->integral = pid->integral_min;
  }

  pid->prev_measurement = measurement;
  pid->prev_error = error;

  // Output is recalculated deliberately using the updated integral to apply
  // post-windup terms.
  float output = p_term + (pid->ki * pid->integral) + d_term;

  if (output > pid->limit_max)
    return pid->limit_max;
  else if (output < pid->limit_min)
    return pid->limit_min;

  return output;
}

float PID_UpdateFromError(PID_Controller_t *pid, float error, float velocity,
                          float dt) {
  if (dt <= 1e-6f)
    return 0.0f;

  if (pid->first_run) {
    pid->prev_measurement = 0.0f;
    pid->first_run = false;
  }

  if (pid->wrap_bound > 0.0f) {
    float period = 2.0f * pid->wrap_bound;
    error = fmodf(error + pid->wrap_bound, period);
    if (error < 0.0f)
      error += period;
    error -= pid->wrap_bound;
  }

  // Pure translation-bound derivative proxy using -velocity
  float derivative = -velocity;

  float p_term = pid->kp * error;
  float d_term = pid->kd * derivative;

  float pre_output = p_term + (pid->ki * pid->integral) + d_term;
  if ((pre_output >= pid->limit_max && error > 0.0f) ||
      (pre_output <= pid->limit_min && error < 0.0f)) {
  } else {
    pid->integral += error * dt;
    if (pid->integral > pid->integral_max)
      pid->integral = pid->integral_max;
    else if (pid->integral < pid->integral_min)
      pid->integral = pid->integral_min;
  }

  pid->prev_error = error;

  // Output is recalculated deliberately using the updated integral
  float output = p_term + (pid->ki * pid->integral) + d_term;

  if (output > pid->limit_max)
    return pid->limit_max;
  else if (output < pid->limit_min)
    return pid->limit_min;

  return output;
}

// Convert global Earth frame position errors to local Body frame errors using
// cached Euler rotations. (Assumes standard intrinsic ZYX rotation matrix
// mapping). Note: Only translational vectors (x,y,z) are rotated. Attitude
// errors (roll, pitch, yaw) are executed directly against body frame
// assumptions cleanly.
void AUV_EarthToBody_Translate(float err_x, float err_y, float err_z,
                               float roll, float pitch, float yaw,
                               float *err_body) {
  float cp = cosf(pitch);
  float sp = sinf(pitch);
  float cy = cosf(yaw);
  float sy = sinf(yaw);
  float cr = cosf(roll);
  float sr = sinf(roll);

  err_body[0] = (cp * cy) * err_x + (cp * sy) * err_y - sp * err_z;
  err_body[1] = (sr * sp * cy - cr * sy) * err_x +
                (sr * sp * sy + cr * cy) * err_y + (sr * cp) * err_z;
  err_body[2] = (cr * sp * cy + sr * sy) * err_x +
                (cr * sp * sy - sr * cy) * err_y + (cr * cp) * err_z;
}

void PID_Calculate(Setpoint_t sp, float *state, float *state_vel, float *tau,
                   PID_Controller_t *pids, float dt) {
  float err_x_earth = sp.x - state[0];
  float err_y_earth = sp.y - state[1];
  float err_z_earth = sp.z - state[2];

  float err_body[3];
  AUV_EarthToBody_Translate(err_x_earth, err_y_earth, err_z_earth, state[3],
                            state[4], state[5], err_body);

  // Pass internally rotated body errors into the PID updates
  // overriding the zeroed positional derivative loss with pure -velocity
  // Note: state_vel from EKF is already rotated correctly prior to passing into
  // PID_Calculate in main.c
  tau[0] = PID_UpdateFromError(&pids[0], err_body[0], state_vel[0], dt);
  tau[1] = PID_UpdateFromError(&pids[1], err_body[1], state_vel[1], dt);
  tau[2] = PID_UpdateFromError(&pids[2], err_body[2], state_vel[2], dt);
  tau[3] = PID_Update(&pids[3], sp.roll, state[3], dt);
  tau[4] = PID_Update(&pids[4], sp.pitch, state[4], dt);
  tau[5] = PID_Update(&pids[5], sp.yaw, state[5], dt);
}

void PID_CalculateSpeed(Setspeed_t ss, float *state_vel, float *tau,
                        PID_Controller_t *pids_vel, float dt) {
  tau[0] = PID_Update(&pids_vel[0], ss.u, state_vel[0], dt);
  tau[1] = PID_Update(&pids_vel[1], ss.v, state_vel[1], dt);
  tau[2] = PID_Update(&pids_vel[2], ss.w, state_vel[2], dt);
  tau[3] = PID_Update(&pids_vel[3], ss.p, state_vel[3], dt);
  tau[4] = PID_Update(&pids_vel[4], ss.q, state_vel[4], dt);
  tau[5] = PID_Update(&pids_vel[5], ss.r, state_vel[5], dt);
}

void PID_CalculateHybrid(Setpoint_t sp, Setspeed_t ss, const bool *use_speed,
                         float *state_pos, float *state_vel, float *tau,
                         PID_Controller_t *pids_pos, PID_Controller_t *pids_vel,
                         Controller_State *ctrl_state, float dt) {
  float err_x_earth = sp.x - state_pos[0];
  float err_y_earth = sp.y - state_pos[1];
  float err_z_earth = sp.z - state_pos[2];

  float err_body[3];
  AUV_EarthToBody_Translate(err_x_earth, err_y_earth, err_z_earth, state_pos[3],
                            state_pos[4], state_pos[5], err_body);

  // The EKF supplies Earth-frame velocities (NED), but both our positional
  // derivative proxy and our explicit speed controller require Body-frame
  // velocities. We reuse the cached rotational math here to natively convert
  // state_vel dynamically.
  float vel_body[3];
  AUV_EarthToBody_Translate(state_vel[0], state_vel[1], state_vel[2],
                            state_pos[3], state_pos[4], state_pos[5], vel_body);

  for (int i = 0; i < 6; i++) {
    // Did the mode just switch for this axis?
    if (use_speed[i] != ctrl_state->prev_use_speed[i]) {
      if (use_speed[i]) {
        PID_Reset(&pids_vel[i]); // Switching to speed, reset speed integral
      } else {
        PID_Reset(
            &pids_pos[i]); // Switching to position, reset position integral
      }
      ctrl_state->prev_use_speed[i] = use_speed[i];
    }
  }

  tau[0] = use_speed[0] ? PID_Update(&pids_vel[0], ss.u, vel_body[0], dt)
                        : PID_UpdateFromError(&pids_pos[0], err_body[0],
                                              vel_body[0], dt);
  tau[1] = use_speed[1] ? PID_Update(&pids_vel[1], ss.v, vel_body[1], dt)
                        : PID_UpdateFromError(&pids_pos[1], err_body[1],
                                              vel_body[1], dt);
  tau[2] = use_speed[2] ? PID_Update(&pids_vel[2], ss.w, vel_body[2], dt)
                        : PID_UpdateFromError(&pids_pos[2], err_body[2],
                                              vel_body[2], dt);
  tau[3] = use_speed[3] ? PID_Update(&pids_vel[3], ss.p, state_vel[3], dt)
                        : PID_Update(&pids_pos[3], sp.roll, state_pos[3], dt);
  tau[4] = use_speed[4] ? PID_Update(&pids_vel[4], ss.q, state_vel[4], dt)
                        : PID_Update(&pids_pos[4], sp.pitch, state_pos[4], dt);
  tau[5] = use_speed[5] ? PID_Update(&pids_vel[5], ss.r, state_vel[5], dt)
                        : PID_Update(&pids_pos[5], sp.yaw, state_pos[5], dt);
}

void Thrust_Allocate(float *tau, float *forces) {
  for (int i = 0; i < AUV_NUM_THRUSTERS; i++) {
    forces[i] = 0.0f;
    for (int j = 0; j < AUV_DOF; j++) {
      forces[i] += T_pinv[i][j] * tau[j];
    }
  }
}

uint16_t Thrust_to_PWM(float thrust_newtons) {
  float max_thrust = AUV_THRUST_MAX;
  float min_thrust = AUV_THRUST_MIN;
  float deadband = AUV_THRUST_DEADBAND;

  if (thrust_newtons > max_thrust)
    thrust_newtons = max_thrust;
  else if (thrust_newtons < min_thrust)
    thrust_newtons = min_thrust;

  uint16_t pwm;
  int32_t pwm_signed;
  if (thrust_newtons > deadband) {
    pwm_signed = (int32_t)(AUV_PWM_CENTER +
                           AUV_THRUST_TO_PWM_COEF_SQRT * sqrtf(thrust_newtons) +
                           AUV_THRUST_TO_PWM_COEF_LIN * thrust_newtons);
  } else if (thrust_newtons < -deadband) {
    pwm_signed =
        (int32_t)(AUV_PWM_CENTER -
                  AUV_THRUST_TO_PWM_COEF_SQRT * sqrtf(fabsf(thrust_newtons)) -
                  AUV_THRUST_TO_PWM_COEF_LIN * fabsf(thrust_newtons));
  } else {
    pwm_signed = AUV_PWM_CENTER;
  }

  pwm = (uint16_t)(pwm_signed < 0 ? 0 : pwm_signed);

  if (pwm > AUV_PWM_MAX)
    pwm = AUV_PWM_MAX;
  if (pwm < AUV_PWM_MIN)
    pwm = AUV_PWM_MIN;

  return pwm;
}

void Process_All_Thrusters(float *forces, uint16_t *pwms, int num) {
  for (int i = 0; i < num; i++) {
    pwms[i] = Thrust_to_PWM(forces[i]);
  }
}
