#include "auvcontrol.h"
#include <math.h>

const float T_pinv[8][6] = {
    { 0.25f,  0.25f,  0.0f,   0.0f,   0.0f,   1.15f},
    { 0.25f, -0.25f,  0.0f,   0.0f,   0.0f,  -1.15f},
    {-0.25f,  0.25f,  0.0f,   0.0f,   0.0f,  -1.15f},
    {-0.25f, -0.25f,  0.0f,   0.0f,   0.0f,   1.15f},
    { 0.0f,   0.0f,   0.25f,  1.15f,  1.15f,  0.0f },
    { 0.0f,   0.0f,   0.25f, -1.15f,  1.15f,  0.0f },
    { 0.0f,   0.0f,   0.25f,  1.15f, -1.15f,  0.0f },
    { 0.0f,   0.0f,   0.25f, -1.15f, -1.15f,  0.0f }
};

void PID_Init(PID_Controller_t *pid, float p, float i, float d, float max, float min, float wrap_bound) {
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

float PID_Update(PID_Controller_t *pid, float setpoint, float measurement, float dt) {
    if (dt <= 1e-6f) return 0.0f;

    if (pid->first_run) {
        pid->prev_measurement = measurement;
        pid->first_run = false;
    }

    float error = setpoint - measurement;
    if (pid->wrap_bound > 0.0f) {
        while (error > pid->wrap_bound) error -= 2.0f * pid->wrap_bound;
        while (error < -pid->wrap_bound) error += 2.0f * pid->wrap_bound;
    }
    
    pid->integral += error * dt;
    
    if (pid->integral > pid->integral_max) pid->integral = pid->integral_max;
    else if (pid->integral < pid->integral_min) pid->integral = pid->integral_min;
    
    float meas_diff = measurement - pid->prev_measurement;
    if (pid->wrap_bound > 0.0f) {
        while (meas_diff > pid->wrap_bound) meas_diff -= 2.0f * pid->wrap_bound;
        while (meas_diff < -pid->wrap_bound) meas_diff += 2.0f * pid->wrap_bound;
    }
    float derivative;
    if (measurement == 0.0f) derivative = (error - pid->prev_error) / dt;
    else derivative = -(measurement - pid->prev_measurement) / dt;
    pid->prev_measurement = measurement;
    pid->prev_error = error;
    
    float output = (pid->kp * error) + (pid->ki * pid->integral) + (pid->kd * derivative);
    
    if (output > pid->limit_max) return pid->limit_max;
    else if (output < pid->limit_min) return pid->limit_min;
    
    return output;
}

// Convert global Earth frame position errors to local Body frame errors using cached Euler rotations
static void compute_err_body(float err_x, float err_y, float err_z, float roll, float pitch, float yaw, float *err_body) {
    float cp = cosf(pitch);
    float sp = sinf(pitch);
    float cy = cosf(yaw);
    float sy = sinf(yaw);
    float cr = cosf(roll);
    float sr = sinf(roll);

    err_body[0] = (cp * cy) * err_x + (cp * sy) * err_y - sp * err_z;
    err_body[1] = (sr * sp * cy - cr * sy) * err_x + (sr * sp * sy + cr * cy) * err_y + (sr * cp) * err_z;
    err_body[2] = (cr * sp * cy + sr * sy) * err_x + (cr * sp * sy - sr * cy) * err_y + (cr * cp) * err_z;
}

void PID_Calculate(Setpoint_t sp, float *state, float *tau, PID_Controller_t *pids, float dt) {
    float err_x_earth = sp.x - state[0];
    float err_y_earth = sp.y - state[1];
    float err_z_earth = sp.z - state[2];
    
    float err_body[3];
    compute_err_body(err_x_earth, err_y_earth, err_z_earth, state[3], state[4], state[5], err_body);
    
    // Pass internally rotated body errors into the PID updates
    // By forcing measurement to 0, we track the pre-transformed error directly.
    tau[0] = PID_Update(&pids[0], err_body[0], 0.0f, dt);
    tau[1] = PID_Update(&pids[1], err_body[1], 0.0f, dt);
    tau[2] = PID_Update(&pids[2], err_body[2], 0.0f, dt);
    tau[3] = PID_Update(&pids[3], sp.roll, state[3], dt);
    tau[4] = PID_Update(&pids[4], sp.pitch, state[4], dt);
    tau[5] = PID_Update(&pids[5], sp.yaw, state[5], dt);
}

void PID_CalculateSpeed(Setspeed_t ss, float *state_vel, float *tau, PID_Controller_t *pids_vel, float dt) {
    tau[0] = PID_Update(&pids_vel[0], ss.u, state_vel[0], dt);
    tau[1] = PID_Update(&pids_vel[1], ss.v, state_vel[1], dt);
    tau[2] = PID_Update(&pids_vel[2], ss.w, state_vel[2], dt);
    tau[3] = PID_Update(&pids_vel[3], ss.p, state_vel[3], dt);
    tau[4] = PID_Update(&pids_vel[4], ss.q, state_vel[4], dt);
    tau[5] = PID_Update(&pids_vel[5], ss.r, state_vel[5], dt);
}

void PID_CalculateHybrid(Setpoint_t sp, Setspeed_t ss, const bool *use_speed, float *state_pos, float *state_vel, float *tau, PID_Controller_t *pids_pos, PID_Controller_t *pids_vel, Controller_State *ctrl_state, float dt) {
    float err_x_earth = sp.x - state_pos[0];
    float err_y_earth = sp.y - state_pos[1];
    float err_z_earth = sp.z - state_pos[2];
    
    float err_body[3];
    compute_err_body(err_x_earth, err_y_earth, err_z_earth, state_pos[3], state_pos[4], state_pos[5], err_body);

    for (int i = 0; i < 6; i++) {
        // Did the mode just switch for this axis?
        if (use_speed[i] != ctrl_state->prev_use_speed[i]) {
            if (use_speed[i]) {
                PID_Reset(&pids_vel[i]); // Switching to speed, reset speed integral
            } else {
                PID_Reset(&pids_pos[i]); // Switching to position, reset position integral
            }
            ctrl_state->prev_use_speed[i] = use_speed[i];
        }
    }

    tau[0] = use_speed[0] ? PID_Update(&pids_vel[0], ss.u, state_vel[0], dt) : PID_Update(&pids_pos[0], err_body[0], 0.0f, dt);
    tau[1] = use_speed[1] ? PID_Update(&pids_vel[1], ss.v, state_vel[1], dt) : PID_Update(&pids_pos[1], err_body[1], 0.0f, dt);
    tau[2] = use_speed[2] ? PID_Update(&pids_vel[2], ss.w, state_vel[2], dt) : PID_Update(&pids_pos[2], err_body[2], 0.0f, dt);
    tau[3] = use_speed[3] ? PID_Update(&pids_vel[3], ss.p, state_vel[3], dt) : PID_Update(&pids_pos[3], sp.roll, state_pos[3], dt);
    tau[4] = use_speed[4] ? PID_Update(&pids_vel[4], ss.q, state_vel[4], dt) : PID_Update(&pids_pos[4], sp.pitch, state_pos[4], dt);
    tau[5] = use_speed[5] ? PID_Update(&pids_vel[5], ss.r, state_vel[5], dt) : PID_Update(&pids_pos[5], sp.yaw, state_pos[5], dt);
}

void PID_CalculateVisualServo(VisualServo_t vs, float *tau, PID_Controller_t *pid_surge, PID_Controller_t *pid_heave, PID_Controller_t *pid_yaw, float dt) {
    // Zero out any unhandled forces (Sway, Roll, Pitch are generally not used to track an object unless it's a very specific application)
    tau[1] = 0.0f; // sway (y)
    tau[3] = 0.0f; // roll
    tau[4] = 0.0f; // pitch
    
    // Safety check: If the target is lost (scale drops to 0), stop moving forward/down.
    // The AUV should only yaw in place to find the target again.
    if (vs.scale <= 0.001f) {
        // Flush any stale integrals built up before the target was lost
        PID_Reset(pid_surge);
        PID_Reset(pid_heave);
        
        tau[0] = 0.0f;
        tau[2] = 0.0f;
        tau[5] = PID_Update(pid_yaw, 0.0f, vs.x_offset, dt);
        return;
    }

    // 1. Yaw Control: Convert X pixel offset on screen to Yaw rotation
    // Positive pixel error (target to the right) -> Positive Yaw torque to spin right
    tau[5] = PID_Update(pid_yaw, 0.0f, vs.x_offset, dt);
    
    // 2. Heave Control: Convert Y pixel offset on screen to Heave (Z)
    // Positive pixel error (target is low on screen) -> Positive Heave (thrust down)
    tau[2] = PID_Update(pid_heave, 0.0f, vs.y_offset, dt);
    
    // 3. Surge Control: Convert Target Scale difference to Surge (X)
    // target_scale is how big we want it. scale is how big it is right now.
    // If target_scale > scale, we need to get closer -> Positive Surge
    tau[0] = PID_Update(pid_surge, vs.target_scale, vs.scale, dt);
}

void Thrust_Allocate(float *tau, float *forces) {
    for (int i = 0; i < 8; i++) {
        forces[i] = 0.0f;
        for (int j = 0; j < 6; j++) {
            forces[i] += T_pinv[i][j] * tau[j];
        }
    }
}

uint16_t Thrust_to_PWM(float thrust_newtons) {
    float max_thrust = 40.0f; 
    float min_thrust = -40.0f;
    float deadband = 0.15f;
    
    if (thrust_newtons > max_thrust) thrust_newtons = max_thrust;
    else if (thrust_newtons < min_thrust) thrust_newtons = min_thrust;
    
    uint16_t pwm;
    if (thrust_newtons > deadband) {
        pwm = (uint16_t)(1500.0f + 12.5f * sqrtf(thrust_newtons) + 3.2f * thrust_newtons);
    } else if (thrust_newtons < -deadband) {
        pwm = (uint16_t)(1500.0f - 12.5f * sqrtf(fabsf(thrust_newtons)) - 3.2f * fabsf(thrust_newtons));
    } else {
        pwm = 1500;
    }

    if (pwm > 1900) pwm = 1900;
    if (pwm < 1100) pwm = 1100;

    return pwm;
}

void Process_All_Thrusters(float *forces, uint16_t *pwms, int num) {
    for (int i = 0; i < num; i++) {
        pwms[i] = Thrust_to_PWM(forces[i]);
    }
}
