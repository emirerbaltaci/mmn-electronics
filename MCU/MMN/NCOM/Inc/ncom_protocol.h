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
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * NCOM C Header File
 * Auto-generated on: 27.02.2026 15:02:40
 * Protocol Version: 0.1
 */

#ifndef INC_NCOM_PROTOCOL_H_
#define INC_NCOM_PROTOCOL_H_

#include <stdint.h>
#include <stddef.h>

#define NCOM_SYNC_BYTE_1 0xA5
#define NCOM_SYNC_BYTE_2 0x5A

#define NCOM_MAX_PAYLOAD_LEN 255
#define NCOM_HEADER_LEN 5
#define NCOM_FOOTER_LEN 2
#define NCOM_OVERHEAD_LEN (NCOM_HEADER_LEN + NCOM_FOOTER_LEN)
#define NCOM_SYNCBYTE_COUNT 2

typedef enum {
    NCOM_MSG_HEARTBEAT = 0,
    NCOM_MSG_SYS_STATUS = 1,
    NCOM_MSG_COMMAND = 2,
    NCOM_MSG_ATTITUDE = 3,
    NCOM_MSG_ROTATION_RATES = 4,
    NCOM_MSG_LINEAR_VELOCITY = 5,
    NCOM_MSG_DEPTH = 6,
    NCOM_MSG_MODE_STATUS = 7,
    NCOM_MSG_TARGETS = 8,
    NCOM_MSG_ACKNOWLEDGEMENT = 9,
    NCOM_MSG_CONFIG_REQ = 40,
    NCOM_MSG_CONFIG_RESP = 41,
    NCOM_MSG_CONFIG_SET_MCU = 42,
    NCOM_MSG_CONFIG_SET_FREERTOS = 43,
    NCOM_MSG_CONFIG_SET_SENSOR = 44,
    NCOM_MSG_CONFIG_SET_ACTUATOR = 45,
    NCOM_MSG_CONFIG_SET_PID = 46,
    NCOM_MSG_CONFIG_SET_EKF = 47,
    NCOM_MSG_CONFIG_REQ_STARTUP = 48,
    NCOM_MSG_CONFIG_SET_STARTUP = 49,
    NCOM_MSG_INIT_ERROR = 200,
    NCOM_MSG_DYING_GASP = 255,
} NCOM_MsgID_t;

#define NCOM_HEARTBEAT_VEHICLE_STATE_INIT 0
#define NCOM_HEARTBEAT_VEHICLE_STATE_DISARMED 1
#define NCOM_HEARTBEAT_VEHICLE_STATE_PREARMED 2
#define NCOM_HEARTBEAT_VEHICLE_STATE_ARMED 3
#define NCOM_HEARTBEAT_VEHICLE_STATE_MISSION 4
#define NCOM_HEARTBEAT_VEHICLE_STATE_DISARMED_MANUAL 11
#define NCOM_HEARTBEAT_VEHICLE_STATE_PREARMED_MANUAL 12
#define NCOM_HEARTBEAT_VEHICLE_STATE_ARMED_MANUAL 13
#define NCOM_HEARTBEAT_VEHICLE_STATE_DISARMED_ERR 21
#define NCOM_HEARTBEAT_VEHICLE_STATE_PREARMED_ERR 22
#define NCOM_HEARTBEAT_VEHICLE_STATE_ARMED_ERR 23
#define NCOM_HEARTBEAT_VEHICLE_STATE_DISARMED_MANUAL_ERR 31
#define NCOM_HEARTBEAT_VEHICLE_STATE_PREARMED_MANUAL_ERR 32
#define NCOM_HEARTBEAT_VEHICLE_STATE_ARMED_MANUAL_ERR 33
#define NCOM_HEARTBEAT_VEHICLE_STATE_MCU_HARDFAULT 255

#define NCOM_FLAG_HEARTBEAT_LEAK_DETECTED (1 << 0)
#define NCOM_FLAG_HEARTBEAT_BATTERY_LOW (1 << 1)
#define NCOM_FLAG_HEARTBEAT_INIT_ERR (1 << 2)
#define NCOM_FLAG_HEARTBEAT_NAV_ERR (1 << 3)
#define NCOM_FLAG_HEARTBEAT_BAR30_ERR (1 << 4)
#define NCOM_FLAG_HEARTBEAT_PERIPHERAL_ERR (1 << 5)
#define NCOM_FLAG_HEARTBEAT_RTOS_TASK_ERR (1 << 6)
#define NCOM_FLAG_HEARTBEAT_MISSION_ABORTED (1 << 7)

#define NCOM_COMMAND_CMD_ID_DISARM 0
#define NCOM_COMMAND_CMD_ID_PREARM 1
#define NCOM_COMMAND_CMD_ID_ARM 2
#define NCOM_COMMAND_CMD_ID_START_MISSION 3
#define NCOM_COMMAND_CMD_ID_ABORT_MISSION 4
#define NCOM_COMMAND_CMD_ID_CLEAR_FLAGS 5
#define NCOM_COMMAND_CMD_ID_SET_MODE 10
#define NCOM_COMMAND_CMD_ID_SET_TARGET_DEPTH_MM 20
#define NCOM_COMMAND_CMD_ID_SET_TARGET_ROLL_DEG 21
#define NCOM_COMMAND_CMD_ID_SET_TARGET_PITCH_DEG 22
#define NCOM_COMMAND_CMD_ID_SET_TARGET_YAW_DEG 23
#define NCOM_COMMAND_CMD_ID_SET_TARGET_SURGE_MM_S 24
#define NCOM_COMMAND_CMD_ID_SET_TARGET_SWAY_MM_S 25
#define NCOM_COMMAND_CMD_ID_SET_TARGET_HEAVE_MM_S 26
#define NCOM_COMMAND_CMD_ID_SET_LIGHTS 30
#define NCOM_COMMAND_CMD_ID_REBOOT_MCU 40
#define NCOM_COMMAND_CMD_ID_CALIBRATE_DEPTH_0 100
#define NCOM_COMMAND_CMD_ID_CALIBRATE_AXES_0 101
#define NCOM_COMMAND_CMD_ID_SET_HEARTBEAT_RATE 200
#define NCOM_COMMAND_CMD_ID_EMERGENCY 255

#define NCOM_FLAG_MODE_STATUS_DISARMED (1 << 0)
#define NCOM_FLAG_MODE_STATUS_PREARMED (1 << 1)
#define NCOM_FLAG_MODE_STATUS_ARMED (1 << 2)
#define NCOM_FLAG_MODE_STATUS_AUTO (1 << 3)
#define NCOM_FLAG_MODE_STATUS_MANUAL (1 << 4)
#define NCOM_FLAG_MODE_STATUS_HOLD_DEPTH (1 << 5)
#define NCOM_FLAG_MODE_STATUS_HOLD_ATTITUDE (1 << 6)
#define NCOM_FLAG_MODE_STATUS_STABILIZE (1 << 7)
#define NCOM_FLAG_MODE_STATUS_MISSION_ACTIVE (1 << 8)

#define NCOM_FLAG_TARGETS_TARGET_DEPTH_IS_ACTIVE (1 << 0)
#define NCOM_FLAG_TARGETS_TARGET_ROLL_IS_ACTIVE (1 << 1)
#define NCOM_FLAG_TARGETS_TARGET_PITCH_IS_ACTIVE (1 << 2)
#define NCOM_FLAG_TARGETS_TARGET_YAW_IS_ACTIVE (1 << 3)

#define NCOM_ACKNOWLEDGEMENT_RESPONSE_NACK 0
#define NCOM_ACKNOWLEDGEMENT_RESPONSE_ACK 1
#define NCOM_ACKNOWLEDGEMENT_RESPONSE_INVALID_PARAMETER 254
#define NCOM_ACKNOWLEDGEMENT_RESPONSE_UNKNOWN_ERROR 255

#define NCOM_CONFIG_REQ_CONFIG_ID_SYS_ID 0
#define NCOM_CONFIG_REQ_CONFIG_ID_COMP_ID 1
#define NCOM_CONFIG_REQ_CONFIG_ID_TELEM_RATE_HZ 2
#define NCOM_CONFIG_REQ_CONFIG_ID_TASK_RATE_TELEM 10
#define NCOM_CONFIG_REQ_CONFIG_ID_TASK_RATE_CTRL 11
#define NCOM_CONFIG_REQ_CONFIG_ID_IMU_RATE_HZ 20
#define NCOM_CONFIG_REQ_CONFIG_ID_BAR30_RATE_HZ 21
#define NCOM_CONFIG_REQ_CONFIG_ID_MAG_RATE_HZ 22
#define NCOM_CONFIG_REQ_CONFIG_ID_PWM_MIN_PULSE 30
#define NCOM_CONFIG_REQ_CONFIG_ID_PWM_MAX_PULSE 31
#define NCOM_CONFIG_REQ_CONFIG_ID_ESC_MODE 32
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_ROLL_P 40
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_ROLL_I 41
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_ROLL_D 42
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_PITCH_P 43
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_PITCH_I 44
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_PITCH_D 45
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_YAW_P 46
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_YAW_I 47
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_YAW_D 48
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_DEPTH_P 49
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_DEPTH_I 50
#define NCOM_CONFIG_REQ_CONFIG_ID_PID_DEPTH_D 51
#define NCOM_CONFIG_REQ_CONFIG_ID_EKF_PROCESS_NOISE 60
#define NCOM_CONFIG_REQ_CONFIG_ID_EKF_MEAS_NOISE 61

#define NCOM_CONFIG_RESP_CONFIG_ID_SYS_ID 0
#define NCOM_CONFIG_RESP_CONFIG_ID_COMP_ID 1
#define NCOM_CONFIG_RESP_CONFIG_ID_TELEM_RATE_HZ 2
#define NCOM_CONFIG_RESP_CONFIG_ID_TASK_RATE_TELEM 10
#define NCOM_CONFIG_RESP_CONFIG_ID_TASK_RATE_CTRL 11
#define NCOM_CONFIG_RESP_CONFIG_ID_IMU_RATE_HZ 20
#define NCOM_CONFIG_RESP_CONFIG_ID_BARO_RATE_HZ 21
#define NCOM_CONFIG_RESP_CONFIG_ID_MAG_RATE_HZ 22
#define NCOM_CONFIG_RESP_CONFIG_ID_PWM_MIN_PULSE 30
#define NCOM_CONFIG_RESP_CONFIG_ID_PWM_MAX_PULSE 31
#define NCOM_CONFIG_RESP_CONFIG_ID_ESC_MODE 32
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_ROLL_P 40
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_ROLL_I 41
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_ROLL_D 42
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_PITCH_P 43
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_PITCH_I 44
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_PITCH_D 45
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_YAW_P 46
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_YAW_I 47
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_YAW_D 48
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_DEPTH_P 49
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_DEPTH_I 50
#define NCOM_CONFIG_RESP_CONFIG_ID_PID_DEPTH_D 51
#define NCOM_CONFIG_RESP_CONFIG_ID_EKF_PROCESS_NOISE 60
#define NCOM_CONFIG_RESP_CONFIG_ID_EKF_MEAS_NOISE 61

#define NCOM_CONFIG_RESP_DATA_TYPE_INT8 0
#define NCOM_CONFIG_RESP_DATA_TYPE_UINT8 1
#define NCOM_CONFIG_RESP_DATA_TYPE_INT16 2
#define NCOM_CONFIG_RESP_DATA_TYPE_UINT16 3
#define NCOM_CONFIG_RESP_DATA_TYPE_INT32 4
#define NCOM_CONFIG_RESP_DATA_TYPE_UINT32 5
#define NCOM_CONFIG_RESP_DATA_TYPE_FLOAT32 6

#define NCOM_CONFIG_SET_MCU_CONFIG_ID_SYS_ID 0
#define NCOM_CONFIG_SET_MCU_CONFIG_ID_COMP_ID 1
#define NCOM_CONFIG_SET_MCU_CONFIG_ID_TELEM_RATE_HZ 2

#define NCOM_CONFIG_SET_MCU_DATA_TYPE_INT8 0
#define NCOM_CONFIG_SET_MCU_DATA_TYPE_UINT8 1
#define NCOM_CONFIG_SET_MCU_DATA_TYPE_INT16 2
#define NCOM_CONFIG_SET_MCU_DATA_TYPE_UINT16 3
#define NCOM_CONFIG_SET_MCU_DATA_TYPE_INT32 4
#define NCOM_CONFIG_SET_MCU_DATA_TYPE_UINT32 5
#define NCOM_CONFIG_SET_MCU_DATA_TYPE_FLOAT32 6

#define NCOM_CONFIG_SET_FREERTOS_CONFIG_ID_TASK_RATE_TELEM 10
#define NCOM_CONFIG_SET_FREERTOS_CONFIG_ID_TASK_RATE_CTRL 11

#define NCOM_CONFIG_SET_FREERTOS_DATA_TYPE_INT8 0
#define NCOM_CONFIG_SET_FREERTOS_DATA_TYPE_UINT8 1
#define NCOM_CONFIG_SET_FREERTOS_DATA_TYPE_INT16 2
#define NCOM_CONFIG_SET_FREERTOS_DATA_TYPE_UINT16 3
#define NCOM_CONFIG_SET_FREERTOS_DATA_TYPE_INT32 4
#define NCOM_CONFIG_SET_FREERTOS_DATA_TYPE_UINT32 5
#define NCOM_CONFIG_SET_FREERTOS_DATA_TYPE_FLOAT32 6

#define NCOM_CONFIG_SET_SENSOR_CONFIG_ID_IMU_RATE_HZ 20
#define NCOM_CONFIG_SET_SENSOR_CONFIG_ID_BARO_RATE_HZ 21
#define NCOM_CONFIG_SET_SENSOR_CONFIG_ID_MAG_RATE_HZ 22

#define NCOM_CONFIG_SET_SENSOR_DATA_TYPE_INT8 0
#define NCOM_CONFIG_SET_SENSOR_DATA_TYPE_UINT8 1
#define NCOM_CONFIG_SET_SENSOR_DATA_TYPE_INT16 2
#define NCOM_CONFIG_SET_SENSOR_DATA_TYPE_UINT16 3
#define NCOM_CONFIG_SET_SENSOR_DATA_TYPE_INT32 4
#define NCOM_CONFIG_SET_SENSOR_DATA_TYPE_UINT32 5
#define NCOM_CONFIG_SET_SENSOR_DATA_TYPE_FLOAT32 6

#define NCOM_CONFIG_SET_ACTUATOR_CONFIG_ID_PWM_MIN_PULSE 30
#define NCOM_CONFIG_SET_ACTUATOR_CONFIG_ID_PWM_MAX_PULSE 31
#define NCOM_CONFIG_SET_ACTUATOR_CONFIG_ID_ESC_MODE 32

#define NCOM_CONFIG_SET_ACTUATOR_DATA_TYPE_INT8 0
#define NCOM_CONFIG_SET_ACTUATOR_DATA_TYPE_UINT8 1
#define NCOM_CONFIG_SET_ACTUATOR_DATA_TYPE_INT16 2
#define NCOM_CONFIG_SET_ACTUATOR_DATA_TYPE_UINT16 3
#define NCOM_CONFIG_SET_ACTUATOR_DATA_TYPE_INT32 4
#define NCOM_CONFIG_SET_ACTUATOR_DATA_TYPE_UINT32 5
#define NCOM_CONFIG_SET_ACTUATOR_DATA_TYPE_FLOAT32 6

#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_ROLL_P 40
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_ROLL_I 41
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_ROLL_D 42
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_PITCH_P 43
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_PITCH_I 44
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_PITCH_D 45
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_YAW_P 46
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_YAW_I 47
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_YAW_D 48
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_DEPTH_P 49
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_DEPTH_I 50
#define NCOM_CONFIG_SET_PID_CONFIG_ID_PID_DEPTH_D 51

#define NCOM_CONFIG_SET_PID_DATA_TYPE_INT8 0
#define NCOM_CONFIG_SET_PID_DATA_TYPE_UINT8 1
#define NCOM_CONFIG_SET_PID_DATA_TYPE_INT16 2
#define NCOM_CONFIG_SET_PID_DATA_TYPE_UINT16 3
#define NCOM_CONFIG_SET_PID_DATA_TYPE_INT32 4
#define NCOM_CONFIG_SET_PID_DATA_TYPE_UINT32 5
#define NCOM_CONFIG_SET_PID_DATA_TYPE_FLOAT32 6

#define NCOM_CONFIG_SET_EKF_CONFIG_ID_EKF_PROCESS_NOISE 60
#define NCOM_CONFIG_SET_EKF_CONFIG_ID_EKF_MEAS_NOISE 61

#define NCOM_CONFIG_SET_EKF_DATA_TYPE_INT8 0
#define NCOM_CONFIG_SET_EKF_DATA_TYPE_UINT8 1
#define NCOM_CONFIG_SET_EKF_DATA_TYPE_INT16 2
#define NCOM_CONFIG_SET_EKF_DATA_TYPE_UINT16 3
#define NCOM_CONFIG_SET_EKF_DATA_TYPE_INT32 4
#define NCOM_CONFIG_SET_EKF_DATA_TYPE_UINT32 5
#define NCOM_CONFIG_SET_EKF_DATA_TYPE_FLOAT32 6

#define NCOM_INIT_ERROR_ERROR_CODE_IMU_INIT_ERR 0
#define NCOM_INIT_ERROR_ERROR_CODE_MAG_INIT_ERR 1
#define NCOM_INIT_ERROR_ERROR_CODE_BAR30_INIT_ERR 2

#define NCOM_FLAG_DYING_GASP_EKF_TASK_DEAD (1 << 0)
#define NCOM_FLAG_DYING_GASP_CONTROL_TASK_DEAD (1 << 1)
#define NCOM_FLAG_DYING_GASP_NCOM_TASK_DEAD (1 << 2)

// ID 0: Sent at 1 Hz
typedef struct __attribute__((packed)) {
    uint8_t device_id;
    uint8_t vehicle_state;
    uint8_t flags;
    uint32_t uptime_ms;
} NCOM_Payload_HEARTBEAT_t;
#define NCOM_LEN_HEARTBEAT 7

// ID 1: Microcontroller status
typedef struct __attribute__((packed)) {
    uint8_t cpu_load_perc;
    uint16_t bat_voltage_mv;
    float mcu_temp_c;
} NCOM_Payload_SYS_STATUS_t;
#define NCOM_LEN_SYS_STATUS 7

// ID 2: Core commands to the MCU from RPi
typedef struct __attribute__((packed)) {
    uint8_t cmd_id;
    int32_t cmd_param0;
    int32_t cmd_param1;
    int32_t cmd_param2;
    int32_t cmd_param3;
    int32_t cmd_param4;
    int32_t cmd_param5;
} NCOM_Payload_COMMAND_t;
#define NCOM_LEN_COMMAND 25

// ID 3: Vehicle orientation in 3-D space
typedef struct __attribute__((packed)) {
    float roll_deg;
    float pitch_deg;
    float yaw_deg;
    uint8_t validity;
} NCOM_Payload_ATTITUDE_t;
#define NCOM_LEN_ATTITUDE 13

// ID 4: Angular rotation rates of the vehicle
typedef struct __attribute__((packed)) {
    float p_deg_s;
    float pitch_deg;
    float yaw_deg;
} NCOM_Payload_ROTATION_RATES_t;
#define NCOM_LEN_ROTATION_RATES 12

// ID 5: Linear velocities of the vehicle
typedef struct __attribute__((packed)) {
    float surge_mm_s;
    float sway_mm_s;
    float heave_mm_s;
} NCOM_Payload_LINEAR_VELOCITY_t;
#define NCOM_LEN_LINEAR_VELOCITY 12

// ID 6: Depth and related information
typedef struct __attribute__((packed)) {
    int32_t depth_mm;
    int32_t depth_rate_mm_s;
    uint8_t depth_hold_is_active;
} NCOM_Payload_DEPTH_t;
#define NCOM_LEN_DEPTH 9

// ID 7: Vehicle mode information
typedef struct __attribute__((packed)) {
    uint16_t mode_flags;
} NCOM_Payload_MODE_STATUS_t;
#define NCOM_LEN_MODE_STATUS 2

// ID 8: Active targets information
typedef struct __attribute__((packed)) {
    uint8_t target_flags;
    int32_t target_depth_mm;
    float target_roll_deg;
    float target_pitch_deg;
    float target_yaw_deg;
} NCOM_Payload_TARGETS_t;
#define NCOM_LEN_TARGETS 17

// ID 9: Acknowledgement message sent after command is received
typedef struct __attribute__((packed)) {
    uint8_t requested_msg_id;
    uint8_t requested_seq;
    uint8_t response;
} NCOM_Payload_ACKNOWLEDGEMENT_t;
#define NCOM_LEN_ACKNOWLEDGEMENT 3

// ID 40: Request to send configuration value
typedef struct __attribute__((packed)) {
    uint16_t config_id;
} NCOM_Payload_CONFIG_REQ_t;
#define NCOM_LEN_CONFIG_REQ 2

// ID 41: Response with configuration value
typedef struct __attribute__((packed)) {
    uint16_t config_id;
    uint8_t data_type;
    int32_t config_value;
} NCOM_Payload_CONFIG_RESP_t;
#define NCOM_LEN_CONFIG_RESP 7

// ID 42: Request to set an MCU layout configuration value
typedef struct __attribute__((packed)) {
    uint16_t config_id;
    uint8_t data_type;
    int32_t config_value;
    uint8_t save_to_flash;
} NCOM_Payload_CONFIG_SET_MCU_t;
#define NCOM_LEN_CONFIG_SET_MCU 8

// ID 43: Request to set a FreeRTOS configuration value
typedef struct __attribute__((packed)) {
    uint16_t config_id;
    uint8_t data_type;
    int32_t config_value;
    uint8_t save_to_flash;
} NCOM_Payload_CONFIG_SET_FREERTOS_t;
#define NCOM_LEN_CONFIG_SET_FREERTOS 8

// ID 44: Request to set a Sensor configuration value
typedef struct __attribute__((packed)) {
    uint16_t config_id;
    uint8_t data_type;
    int32_t config_value;
    uint8_t save_to_flash;
} NCOM_Payload_CONFIG_SET_SENSOR_t;
#define NCOM_LEN_CONFIG_SET_SENSOR 8

// ID 45: Request to set an Actuator configuration value
typedef struct __attribute__((packed)) {
    uint16_t config_id;
    uint8_t data_type;
    int32_t config_value;
    uint8_t save_to_flash;
} NCOM_Payload_CONFIG_SET_ACTUATOR_t;
#define NCOM_LEN_CONFIG_SET_ACTUATOR 8

// ID 46: Request to set a PID loop configuration value
typedef struct __attribute__((packed)) {
    uint16_t config_id;
    uint8_t data_type;
    int32_t config_value;
    uint8_t save_to_flash;
} NCOM_Payload_CONFIG_SET_PID_t;
#define NCOM_LEN_CONFIG_SET_PID 8

// ID 47: Request to set an EKF configuration value
typedef struct __attribute__((packed)) {
    uint16_t config_id;
    uint8_t data_type;
    int32_t config_value[9];
} NCOM_Payload_CONFIG_SET_EKF_t;
#define NCOM_LEN_CONFIG_SET_EKF 39

// ID 48: Request saved config file after system reset
typedef struct __attribute__((packed)) {
} NCOM_Payload_CONFIG_REQ_STARTUP_t;
#define NCOM_LEN_CONFIG_REQ_STARTUP 0

// ID 49: Send config file data to MCU
typedef struct __attribute__((packed)) {
} NCOM_Payload_CONFIG_SET_STARTUP_t;
#define NCOM_LEN_CONFIG_SET_STARTUP 0

// ID 200: Sent when an error occurs during initialization
typedef struct __attribute__((packed)) {
    uint8_t error_code;
} NCOM_Payload_INIT_ERROR_t;
#define NCOM_LEN_INIT_ERROR 1

// ID 255: Sent when a critical task does not respond. The MCU then undergoes IWDG reset
typedef struct __attribute__((packed)) {
    uint8_t dead_tasks;
} NCOM_Payload_DYING_GASP_t;
#define NCOM_LEN_DYING_GASP 1

// Pack/Unpack Functions
// Return value is the size of the payload
size_t ncom_pack_heartbeat(uint8_t *buf, const NCOM_Payload_HEARTBEAT_t *msg);
size_t ncom_unpack_heartbeat(const uint8_t *buf, NCOM_Payload_HEARTBEAT_t *msg);
size_t ncom_pack_sys_status(uint8_t *buf, const NCOM_Payload_SYS_STATUS_t *msg);
size_t ncom_unpack_sys_status(const uint8_t *buf, NCOM_Payload_SYS_STATUS_t *msg);
size_t ncom_pack_command(uint8_t *buf, const NCOM_Payload_COMMAND_t *msg);
size_t ncom_unpack_command(const uint8_t *buf, NCOM_Payload_COMMAND_t *msg);
size_t ncom_pack_attitude(uint8_t *buf, const NCOM_Payload_ATTITUDE_t *msg);
size_t ncom_unpack_attitude(const uint8_t *buf, NCOM_Payload_ATTITUDE_t *msg);
size_t ncom_pack_rotation_rates(uint8_t *buf, const NCOM_Payload_ROTATION_RATES_t *msg);
size_t ncom_unpack_rotation_rates(const uint8_t *buf, NCOM_Payload_ROTATION_RATES_t *msg);
size_t ncom_pack_linear_velocity(uint8_t *buf, const NCOM_Payload_LINEAR_VELOCITY_t *msg);
size_t ncom_unpack_linear_velocity(const uint8_t *buf, NCOM_Payload_LINEAR_VELOCITY_t *msg);
size_t ncom_pack_depth(uint8_t *buf, const NCOM_Payload_DEPTH_t *msg);
size_t ncom_unpack_depth(const uint8_t *buf, NCOM_Payload_DEPTH_t *msg);
size_t ncom_pack_mode_status(uint8_t *buf, const NCOM_Payload_MODE_STATUS_t *msg);
size_t ncom_unpack_mode_status(const uint8_t *buf, NCOM_Payload_MODE_STATUS_t *msg);
size_t ncom_pack_targets(uint8_t *buf, const NCOM_Payload_TARGETS_t *msg);
size_t ncom_unpack_targets(const uint8_t *buf, NCOM_Payload_TARGETS_t *msg);
size_t ncom_pack_acknowledgement(uint8_t *buf, const NCOM_Payload_ACKNOWLEDGEMENT_t *msg);
size_t ncom_unpack_acknowledgement(const uint8_t *buf, NCOM_Payload_ACKNOWLEDGEMENT_t *msg);
size_t ncom_pack_config_req(uint8_t *buf, const NCOM_Payload_CONFIG_REQ_t *msg);
size_t ncom_unpack_config_req(const uint8_t *buf, NCOM_Payload_CONFIG_REQ_t *msg);
size_t ncom_pack_config_resp(uint8_t *buf, const NCOM_Payload_CONFIG_RESP_t *msg);
size_t ncom_unpack_config_resp(const uint8_t *buf, NCOM_Payload_CONFIG_RESP_t *msg);
size_t ncom_pack_config_set_mcu(uint8_t *buf, const NCOM_Payload_CONFIG_SET_MCU_t *msg);
size_t ncom_unpack_config_set_mcu(const uint8_t *buf, NCOM_Payload_CONFIG_SET_MCU_t *msg);
size_t ncom_pack_config_set_freertos(uint8_t *buf, const NCOM_Payload_CONFIG_SET_FREERTOS_t *msg);
size_t ncom_unpack_config_set_freertos(const uint8_t *buf, NCOM_Payload_CONFIG_SET_FREERTOS_t *msg);
size_t ncom_pack_config_set_sensor(uint8_t *buf, const NCOM_Payload_CONFIG_SET_SENSOR_t *msg);
size_t ncom_unpack_config_set_sensor(const uint8_t *buf, NCOM_Payload_CONFIG_SET_SENSOR_t *msg);
size_t ncom_pack_config_set_actuator(uint8_t *buf, const NCOM_Payload_CONFIG_SET_ACTUATOR_t *msg);
size_t ncom_unpack_config_set_actuator(const uint8_t *buf, NCOM_Payload_CONFIG_SET_ACTUATOR_t *msg);
size_t ncom_pack_config_set_pid(uint8_t *buf, const NCOM_Payload_CONFIG_SET_PID_t *msg);
size_t ncom_unpack_config_set_pid(const uint8_t *buf, NCOM_Payload_CONFIG_SET_PID_t *msg);
size_t ncom_pack_config_set_ekf(uint8_t *buf, const NCOM_Payload_CONFIG_SET_EKF_t *msg);
size_t ncom_unpack_config_set_ekf(const uint8_t *buf, NCOM_Payload_CONFIG_SET_EKF_t *msg);
size_t ncom_pack_config_req_startup(uint8_t *buf, const NCOM_Payload_CONFIG_REQ_STARTUP_t *msg);
size_t ncom_unpack_config_req_startup(const uint8_t *buf, NCOM_Payload_CONFIG_REQ_STARTUP_t *msg);
size_t ncom_pack_config_set_startup(uint8_t *buf, const NCOM_Payload_CONFIG_SET_STARTUP_t *msg);
size_t ncom_unpack_config_set_startup(const uint8_t *buf, NCOM_Payload_CONFIG_SET_STARTUP_t *msg);
size_t ncom_pack_init_error(uint8_t *buf, const NCOM_Payload_INIT_ERROR_t *msg);
size_t ncom_unpack_init_error(const uint8_t *buf, NCOM_Payload_INIT_ERROR_t *msg);
size_t ncom_pack_dying_gasp(uint8_t *buf, const NCOM_Payload_DYING_GASP_t *msg);
size_t ncom_unpack_dying_gasp(const uint8_t *buf, NCOM_Payload_DYING_GASP_t *msg);

#endif /* INC_NCOM_PROTOCOL_H_ */
