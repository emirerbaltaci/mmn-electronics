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

#ifndef AUVCONTROL_H
#define AUVCONTROL_H

#include <stdbool.h>
#include <stdint.h>

#define AUV_NUM_THRUSTERS 8
#define AUV_DOF 6

typedef struct {
  float x;
  float y;
  float z;
  float roll;
  float pitch;
  float yaw;
} Setpoint_t;

typedef struct {
  float u;
  float v;
  float w;
  float p;
  float q;
  float r;
} Setspeed_t;

typedef struct {
  float kp;
  float ki;
  float kd;
  float integral;
  float prev_error;
  float prev_measurement;
  float limit_max;
  float limit_min;
  float integral_max;
  float integral_min;
  float wrap_bound;
  bool first_run;
} PID_Controller_t;

typedef struct {
  bool prev_use_speed[AUV_DOF];
} Controller_State;

void PID_Init(PID_Controller_t *pid, float p, float i, float d, float max,
              float min, float wrap_bound);
void PID_Reset(PID_Controller_t *pid);
float PID_Update(PID_Controller_t *pid, float setpoint, float measurement,
                 float dt);
float PID_UpdateFromError(PID_Controller_t *pid, float error, float velocity,
                          float dt);
void PID_Calculate(Setpoint_t sp, float *state, float *state_vel, float *tau,
                   PID_Controller_t *pids, float dt);
void PID_CalculateSpeed(Setspeed_t ss, float *state_vel, float *tau,
                        PID_Controller_t *pids_vel, float dt);
void PID_CalculateHybrid(Setpoint_t sp, Setspeed_t ss, const bool *use_speed,
                         float *state_pos, float *state_vel, float *tau,
                         PID_Controller_t *pids_pos, PID_Controller_t *pids_vel,
                         Controller_State *ctrl_state, float dt);

void AUV_EarthToBody_Translate(float err_x, float err_y, float err_z,
                               float roll, float pitch, float yaw,
                               float *err_body);

void Thrust_Allocate(float *tau, float *forces);
uint16_t Thrust_to_PWM(float thrust_newtons);
void Process_All_Thrusters(float *forces, uint16_t *pwms, int num);

#endif // AUVCONTROL_H
