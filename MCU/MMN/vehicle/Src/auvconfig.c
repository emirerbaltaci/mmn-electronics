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

#include "auvconfig.h"
#include "FreeRTOS.h"
#include "bar30.h"
#include "semphr.h"
#include <math.h>

AUV_Config_t auvConfig;
static SemaphoreHandle_t xConfigMutex = NULL;

void AUV_Config_Init(void) {
  // EKF Parameters
  auvConfig.ekf.imu_lever_arm_x = EKF_IMU_LEVER_ARM_X;
  auvConfig.ekf.imu_lever_arm_y = EKF_IMU_LEVER_ARM_Y;
  auvConfig.ekf.imu_lever_arm_z = EKF_IMU_LEVER_ARM_Z;
  auvConfig.ekf.mag_prescaler = EKF_MAG_PRESCALER;
  auvConfig.ekf.gravity_norm_min = EKF_GRAVITY_NORM_MIN;
  auvConfig.ekf.gravity_norm_max = EKF_GRAVITY_NORM_MAX;
  auvConfig.ekf.r_accel = EKF_R_ACCEL;
  auvConfig.ekf.r_mag = EKF_R_MAG;
  auvConfig.ekf.r_baro = EKF_R_BARO;
  auvConfig.ekf.r_zupt = EKF_R_ZUPT;
  auvConfig.ekf.p_init_pos = EKF_P_INIT_POS;
  auvConfig.ekf.p_init_vel = EKF_P_INIT_VEL;
  auvConfig.ekf.p_init_att = EKF_P_INIT_ATT;
  auvConfig.ekf.p_init_bg = EKF_P_INIT_BG;
  auvConfig.ekf.p_init_ba = EKF_P_INIT_BA;
  auvConfig.ekf.q_vel_noise = EKF_Q_VEL_NOISE;
  auvConfig.ekf.q_att_noise = EKF_Q_ATT_NOISE;
  auvConfig.ekf.q_bg_noise = EKF_Q_BG_NOISE;
  auvConfig.ekf.q_ba_noise = EKF_Q_BA_NOISE;
  auvConfig.ekf.earth_mag_x = EKF_EARTH_MAG_X;
  auvConfig.ekf.earth_mag_y = EKF_EARTH_MAG_Y;
  auvConfig.ekf.earth_mag_z = EKF_EARTH_MAG_Z;
  auvConfig.ekf.chi_square_th_dof1 = EKF_CHI_SQUARE_TH_DOF1;
  auvConfig.ekf.chi_square_th_dof2 = EKF_CHI_SQUARE_TH_DOF2;
  auvConfig.ekf.chi_square_th_dof3 = EKF_CHI_SQUARE_TH_DOF3;
  auvConfig.ekf.chi_square_th_dof6 = EKF_CHI_SQUARE_TH_DOF6;

  // Generic X/Y Position Default Init (using SP defaults)
  auvConfig.pid.xy.p = AUV_DEFAULT_PID_SP_P;
  auvConfig.pid.xy.i = AUV_DEFAULT_PID_SP_I;
  auvConfig.pid.xy.d = AUV_DEFAULT_PID_SP_D;
  auvConfig.pid.xy.maxout = AUV_DEFAULT_PID_SP_MAXOUT;
  auvConfig.pid.xy.minout = AUV_DEFAULT_PID_SP_MINOUT;
  auvConfig.pid.xy.wrapbound = AUV_DEFAULT_PID_SP_WRAPBOUND;

  // Depth Default Init (using SP defaults)
  auvConfig.pid.depth.p = AUV_DEFAULT_PID_SP_P;
  auvConfig.pid.depth.i = AUV_DEFAULT_PID_SP_I;
  auvConfig.pid.depth.d = AUV_DEFAULT_PID_SP_D;
  auvConfig.pid.depth.maxout = AUV_DEFAULT_PID_SP_MAXOUT;
  auvConfig.pid.depth.minout = AUV_DEFAULT_PID_SP_MINOUT;
  auvConfig.pid.depth.wrapbound = AUV_DEFAULT_PID_SP_WRAPBOUND;

  // Setspeed Default Init (using SS defaults)
  auvConfig.pid.setspeed.p = AUV_DEFAULT_PID_SS_P;
  auvConfig.pid.setspeed.i = AUV_DEFAULT_PID_SS_I;
  auvConfig.pid.setspeed.d = AUV_DEFAULT_PID_SS_D;
  auvConfig.pid.setspeed.maxout = AUV_DEFAULT_PID_SS_MAXOUT;
  auvConfig.pid.setspeed.minout = AUV_DEFAULT_PID_SS_MINOUT;
  auvConfig.pid.setspeed.wrapbound = AUV_DEFAULT_PID_SS_WRAPBOUND;

  // Roll Default Init
  auvConfig.pid.roll = auvConfig.pid.xy;

  // Pitch Default Init
  auvConfig.pid.pitch = auvConfig.pid.xy;

  // Yaw Default Init
  auvConfig.pid.yaw = auvConfig.pid.xy;
  auvConfig.pid.yaw.wrapbound = (float)M_PI;

  // Thruster Parameters
  float tam[8][6] = AUV_TAM_MATRIX;
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 6; j++) {
      auvConfig.thruster.tam_matrix[i][j] = tam[i][j];
    }
  }
  auvConfig.thruster.max_thrust = AUV_THRUST_MAX;
  auvConfig.thruster.min_thrust = AUV_THRUST_MIN;
  auvConfig.thruster.deadband = AUV_THRUST_DEADBAND;
  auvConfig.thruster.pwm_center = AUV_PWM_CENTER;
  auvConfig.thruster.pwm_max = AUV_PWM_MAX;
  auvConfig.thruster.pwm_min = AUV_PWM_MIN;
  auvConfig.thruster.pwm_coef_sqrt = AUV_THRUST_TO_PWM_COEF_SQRT;
  auvConfig.thruster.pwm_coef_lin = AUV_THRUST_TO_PWM_COEF_LIN;

  // Task Parameters
  auvConfig.task.stateestimate_sleep_ms = TASK_STATEESTIMATE_SLEEP_MS;
  auvConfig.task.control_sleep_ms = TASK_CONTROL_SLEEP_MS;
  auvConfig.task.ncom_sleep_ms = TASK_NCOM_SLEEP_MS;
  auvConfig.task.ncom_packet_timeout_ms = TASK_NCOM_PACKET_TIMEOUT_MS;
  auvConfig.task.ncom_handshake_timeout_ms = TASK_NCOM_HANDSHAKE_TIMEOUT_MS;
  auvConfig.task.sensor_sleep_ms = TASK_SENSOR_SLEEP_MS;
  auvConfig.task.sysmonitor_sleep_ms = TASK_SYSMONITOR_SLEEP_MS;
  auvConfig.task.control_pid_dt = TASK_CONTROL_PID_DT;
  auvConfig.task.stateestimate_miss_tol = TASK_STATEESTIMATE_MISS_TOL;
  auvConfig.task.control_miss_tol = TASK_CONTROL_MISS_TOL;
  auvConfig.task.ncom_miss_tol = TASK_NCOM_MISS_TOL;

  // IMU Parameters
  auvConfig.imu.interface = IMU_SETUP_INTERFACE;
  auvConfig.imu.spi_mode = IMU_SETUP_SPI_MODE;
  auvConfig.imu.spi_wire = IMU_SETUP_SPI_WIRE;
  auvConfig.imu.i3c_bus_mode = IMU_SETUP_I3C_BUS_MODE;
  auvConfig.imu.i3c_mode = IMU_SETUP_I3C_MODE;
  auvConfig.imu.clksel = IMU_SETUP_CLKSEL;
  auvConfig.imu.idlemode = IMU_SETUP_IDLEMODE;
  auvConfig.imu.pin9_function = IMU_SETUP_PIN9_FUNCTION;

  auvConfig.imu.accel_mode = IMU_SETUP_ACCEL_MODE;
  auvConfig.imu.accel_fs = IMU_SETUP_ACCEL_FS;
  auvConfig.imu.accel_odr = IMU_SETUP_ACCEL_ODR;
  auvConfig.imu.accel_lpclk = IMU_SETUP_ACCEL_LPCLK;

  auvConfig.imu.gyro_mode = IMU_SETUP_GYRO_MODE;
  auvConfig.imu.gyro_fs = IMU_SETUP_GYRO_FS;
  auvConfig.imu.gyro_odr = IMU_SETUP_GYRO_ODR;

  auvConfig.imu.temp_mode = IMU_SETUP_TEMP_MODE;

  auvConfig.imu.fifo_mode = IMU_SETUP_FIFO_MODE;
  auvConfig.imu.fifo_packet = IMU_SETUP_FIFO_PACKET;
  auvConfig.imu.fifo_hold_last_data = IMU_SETUP_FIFO_HOLD_LAST_DATA;
  auvConfig.imu.fifo_wm_gt_th = IMU_SETUP_FIFO_WM_GT_TH;
  auvConfig.imu.fifo_wm = IMU_SETUP_FIFO_WM;

  auvConfig.imu.tmst_en = IMU_SETUP_TMST_EN;
  auvConfig.imu.tmst_fsync_en = IMU_SETUP_TMST_FSYNC_EN;
  auvConfig.imu.tmst_res = IMU_SETUP_TMST_RES;
  auvConfig.imu.tmst_delta_en = IMU_SETUP_TMST_DELTA_EN;
  auvConfig.imu.tmst_to_regs_en = IMU_SETUP_TMST_TO_REGS_EN;
  auvConfig.imu.fsync_polarity = IMU_SETUP_FSYNC_POLARITY;
  auvConfig.imu.fsync_ui_flag_clear_sel = IMU_SETUP_FSYNC_UI_FLAG_CLEAR_SEL;
  auvConfig.imu.fsync_ui_sel = IMU_SETUP_FSYNC_UI_SEL;

  auvConfig.imu.int1_mode = IMU_SETUP_INT1_MODE;
  auvConfig.imu.int2_mode = IMU_SETUP_INT2_MODE;
  auvConfig.imu.int1_drive_circuit = IMU_SETUP_INT1_DRIVE_CIRCUIT;
  auvConfig.imu.int2_drive_circuit = IMU_SETUP_INT2_DRIVE_CIRCUIT;
  auvConfig.imu.int1_polarity = IMU_SETUP_INT1_POLARITY;
  auvConfig.imu.int2_polarity = IMU_SETUP_INT2_POLARITY;
  auvConfig.imu.int1_src = IMU_SETUP_INT1_SRC;
  auvConfig.imu.int2_src = IMU_SETUP_INT2_SRC;
  auvConfig.imu.int_config0 = IMU_SETUP_INT_CONFIG0;
  auvConfig.imu.int_config1 = IMU_SETUP_INT_CONFIG1;

  auvConfig.imu.accel_aaf_en = IMU_SETUP_ACCEL_AAF_EN;
  auvConfig.imu.accel_aaf_delt = IMU_SETUP_ACCEL_AAF_DELT;
  auvConfig.imu.gyro_aaf_en = IMU_SETUP_GYRO_AAF_EN;
  auvConfig.imu.gyro_aaf_delt = IMU_SETUP_GYRO_AAF_DELT;

  auvConfig.imu.accel_ui_filt_ord = IMU_SETUP_ACCEL_UI_FILT_ORD;
  auvConfig.imu.accel_ui_filt_bw = IMU_SETUP_ACCEL_UI_FILT_BW;
  auvConfig.imu.gyro_ui_filt_ord = IMU_SETUP_GYRO_UI_FILT_ORD;
  auvConfig.imu.gyro_ui_filt_bw = IMU_SETUP_GYRO_UI_FILT_BW;
  auvConfig.imu.temp_filt_bw = IMU_SETUP_TEMP_FILT_BW;

  auvConfig.imu.gyro_nf_en = IMU_SETUP_GYRO_NF_EN;
  auvConfig.imu.gyro_nf_bw = IMU_SETUP_GYRO_NF_BW;
  auvConfig.imu.gyro_nf_clkdiv = IMU_SETUP_GYRO_NF_CLKDIV;
  auvConfig.imu.gyro_nf_x_freq = IMU_SETUP_GYRO_NF_X_FREQ;
  auvConfig.imu.gyro_nf_y_freq = IMU_SETUP_GYRO_NF_Y_FREQ;
  auvConfig.imu.gyro_nf_z_freq = IMU_SETUP_GYRO_NF_Z_FREQ;

  auvConfig.imu.sensor_config0 = IMU_SETUP_SENSOR_CONFIG0;

  auvConfig.imu.rtc_mode = IMU_SETUP_RTC_MODE;
  auvConfig.imu.sensor_endian = IMU_SETUP_SENSOR_ENDIAN;
  auvConfig.imu.spi_slew = IMU_SETUP_SPI_SLEW;
  auvConfig.imu.i2c_slew = IMU_SETUP_I2C_SLEW;
  auvConfig.imu.fifo_count_endian = IMU_SETUP_FIFO_COUNT_ENDIAN;
  auvConfig.imu.fifo_count_rec = IMU_SETUP_FIFO_COUNT_REC;
  auvConfig.imu.fifo_partial_rd = IMU_SETUP_FIFO_PARTIAL_RD;

  // Magnetometer Parameters
  auvConfig.mag.interface = MAG_SETUP_INTERFACE;
  auvConfig.mag.mode = MAG_SETUP_MODE;
  auvConfig.mag.odr = MAG_SETUP_ODR;
  auvConfig.mag.pinfunc = MAG_SETUP_PINFUNC;
  auvConfig.mag.lp = MAG_SETUP_LP;
  auvConfig.mag.comp_temp = MAG_SETUP_COMP_TEMP;
  auvConfig.mag.lpf = MAG_SETUP_LPF;
  auvConfig.mag.off_canc = MAG_SETUP_OFF_CANC;
  auvConfig.mag.set_freq = MAG_SETUP_SET_FREQ;
  auvConfig.mag.off_canc_one_shot = MAG_SETUP_OFF_CANC_ONE_SHOT;
  auvConfig.mag.ble = MAG_SETUP_BLE;
  auvConfig.mag.bdu = MAG_SETUP_BDU;

  auvConfig.mag.int_en = MAG_SETUP_INT;
  auvConfig.mag.int_polarity = MAG_SETUP_INT_POLARITY;
  auvConfig.mag.int_on_dataoff = MAG_SETUP_INT_ON_DATAOFF;
  auvConfig.mag.int_mode = MAG_SETUP_INT_MODE;
  auvConfig.mag.int_th = MAG_SETUP_INT_TH;
  auvConfig.mag.int_src = MAG_SETUP_INT_SRC;

  auvConfig.bar30.osr = BAR30_SETUP_OSR;
  auvConfig.bar30.density = BAR30_SETUP_FLUID_DENSITY;

  /* Create the config mutex — must be called before scheduler starts */
  xConfigMutex = xSemaphoreCreateMutex();
  configASSERT(xConfigMutex);
}

bool AUV_Config_Lock(uint32_t timeout_ms) {
  if (xConfigMutex == NULL)
    return false;
  return xSemaphoreTake(xConfigMutex, pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}

void AUV_Config_Unlock(void) {
  if (xConfigMutex != NULL)
    xSemaphoreGive(xConfigMutex);
}
