#ifndef VEHICLE_INC_AUVCONFIG_H_
#define VEHICLE_INC_AUVCONFIG_H_

#include "ekf_config.h"
#include "imu_config.h"
#include "magnetometer_config.h"
#include "pid_config.h"
#include "task_config.h"
#include "thruster_config.h"
#include <stdint.h>


typedef struct {
  float imu_lever_arm_x;
  float imu_lever_arm_y;
  float imu_lever_arm_z;
  uint32_t mag_prescaler;
  float gravity_norm_min;
  float gravity_norm_max;
  float r_accel;
  float r_mag;
  float r_baro;
  float r_zupt;
  float p_init_pos;
  float p_init_vel;
  float p_init_att;
  float p_init_bg;
  float p_init_ba;
  float q_vel_noise;
  float q_att_noise;
  float q_bg_noise;
  float q_ba_noise;
  float earth_mag_x;
  float earth_mag_y;
  float earth_mag_z;
  float chi_square_th_dof1;
  float chi_square_th_dof2;
  float chi_square_th_dof3;
  float chi_square_th_dof6;
} EKF_ConfigParams_t;

typedef struct {
  float p;
  float i;
  float d;
  float maxout;
  float minout;
  float wrapbound;
} PID_Tuning_t;

typedef struct {
  PID_Tuning_t sp; // Spatial Position
  PID_Tuning_t ss; // Steady State / Speed
} PID_ConfigParams_t;

typedef struct {
  float tam_matrix[8][6];
  float max_thrust;
  float min_thrust;
  float deadband;
  uint16_t pwm_center;
  uint16_t pwm_max;
  uint16_t pwm_min;
  float pwm_coef_sqrt;
  float pwm_coef_lin;
} Thruster_ConfigParams_t;

typedef struct {
  uint32_t stateestimate_sleep_ms;
  uint32_t control_sleep_ms;
  uint32_t ncom_sleep_ms;
  uint32_t sensor_sleep_ms;
  uint32_t sysmonitor_sleep_ms;
  float control_pid_dt;
} Task_ConfigParams_t;

typedef struct {
  uint8_t interface;
  uint8_t spi_mode;
  uint8_t spi_wire;
  uint8_t i3c_bus_mode;
  uint8_t i3c_mode;
  uint8_t clksel;
  uint8_t idlemode;
  uint8_t pin9_function;

  uint8_t accel_mode;
  uint8_t accel_fs;
  uint8_t accel_odr;
  uint8_t accel_lpclk;

  uint8_t gyro_mode;
  uint8_t gyro_fs;
  uint8_t gyro_odr;

  uint8_t temp_mode;

  uint8_t fifo_mode;
  uint8_t fifo_packet;
  uint8_t fifo_hold_last_data;
  uint8_t fifo_wm_gt_th;
  uint16_t fifo_wm;

  uint8_t tmst_en;
  uint8_t tmst_fsync_en;
  uint8_t tmst_res;
  uint8_t tmst_delta_en;
  uint8_t tmst_to_regs_en;
  uint8_t fsync_polarity;
  uint8_t fsync_ui_flag_clear_sel;
  uint8_t fsync_ui_sel;

  uint8_t int1_mode;
  uint8_t int2_mode;
  uint8_t int1_drive_circuit;
  uint8_t int2_drive_circuit;
  uint8_t int1_polarity;
  uint8_t int2_polarity;
  uint8_t int1_src;
  uint8_t int2_src;
  uint8_t int_config0;
  uint8_t int_config1;

  uint8_t accel_aaf_en;
  uint16_t accel_aaf_delt;
  uint8_t gyro_aaf_en;
  uint16_t gyro_aaf_delt;

  uint8_t accel_ui_filt_ord;
  uint8_t accel_ui_filt_bw;
  uint8_t gyro_ui_filt_ord;
  uint8_t gyro_ui_filt_bw;
  uint8_t temp_filt_bw;

  uint8_t gyro_nf_en;
  uint8_t gyro_nf_bw;
  uint8_t gyro_nf_clkdiv;
  uint16_t gyro_nf_x_freq;
  uint16_t gyro_nf_y_freq;
  uint16_t gyro_nf_z_freq;

  uint8_t sensor_config0;

  uint8_t rtc_mode;
  uint8_t sensor_endian;
  uint8_t spi_slew;
  uint8_t i2c_slew;
  uint8_t fifo_count_endian;
  uint8_t fifo_count_rec;
  uint8_t fifo_partial_rd;
} IMU_ConfigParams_t;

typedef struct {
  uint8_t interface;
  uint8_t mode;
  uint8_t odr;
  uint8_t pinfunc;
  uint8_t lp;
  uint8_t comp_temp;
  uint8_t lpf;
  uint8_t off_canc;
  uint8_t set_freq;
  uint8_t off_canc_one_shot;
  uint8_t ble;
  uint8_t bdu;

  uint8_t int_en;
  uint8_t int_polarity;
  uint8_t int_on_dataoff;
  uint8_t int_mode;
  uint16_t int_th;
  uint8_t int_src;
} MAG_ConfigParams_t;

typedef struct {
  EKF_ConfigParams_t ekf;
  PID_ConfigParams_t pid;
  Thruster_ConfigParams_t thruster;
  Task_ConfigParams_t task;
  IMU_ConfigParams_t imu;
  MAG_ConfigParams_t mag;
} AUV_Config_t;

extern AUV_Config_t auvConfig;

void AUV_Config_Init(void);

#endif /* VEHICLE_INC_AUVCONFIG_H_ */
