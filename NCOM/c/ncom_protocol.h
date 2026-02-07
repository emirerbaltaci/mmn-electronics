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
 * Auto-generated on: 07.02.2026 20:47:11
 * Protocol Version: 0.1
 */

#ifndef INC_NCOM_DEF_H_
#define INC_NCOM_DEF_H_

#include <stdint.h>
#include <stddef.h>

#define NCOM_SYNC_BYTE 0xA5

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
    NCOM_MSG_UNUSED = 9,
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
#define NCOM_COMMAND_CMD_ID_SET_MODE_AUTO 10
#define NCOM_COMMAND_CMD_ID_SET_MODE_MANUAL 11
#define NCOM_COMMAND_CMD_ID_SET_MODE_HOLD_DEPTH 12
#define NCOM_COMMAND_CMD_ID_SET_MODE_HOLD_ATTITUDE 13
#define NCOM_COMMAND_CMD_ID_SET_MODE_STABILIZE 14
#define NCOM_COMMAND_CMD_ID_SET_TARGET_DEPTH_MM 20
#define NCOM_COMMAND_CMD_ID_SET_TARGET_ROLL_DEG 21
#define NCOM_COMMAND_CMD_ID_SET_TARGET_PITCH_DEG 22
#define NCOM_COMMAND_CMD_ID_SET_TARGET_YAW_DEG 23
#define NCOM_COMMAND_CMD_ID_SET_TARGET_SURGE_MM_S 24
#define NCOM_COMMAND_CMD_ID_SET_TARGET_SWAY_MM_S 25
#define NCOM_COMMAND_CMD_ID_SET_TARGET_HEAVE_MM_S 26
#define NCOM_COMMAND_CMD_ID_SET_LIGHTS_MAIN 30
#define NCOM_COMMAND_CMD_ID_SET_LIGHTS_AUX 31
#define NCOM_COMMAND_CMD_ID_REBOOT_MCU 40
#define NCOM_COMMAND_CMD_ID_CALIBRATE_DEPTH_0 100
#define NCOM_COMMAND_CMD_ID_CALIBRATE_AXES_0 101
#define NCOM_COMMAND_CMD_ID_SET_HEARTBEAT_RATE 200
#define NCOM_COMMAND_CMD_ID_EMERGENCY_SURFACE 253
#define NCOM_COMMAND_CMD_ID_EMERGENCY_STOP 254
#define NCOM_COMMAND_CMD_ID_EMERGENCY_SOFTKILL 255

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

// ID 0: Sent at 1 Hz
typedef struct __attribute__((packed)) {
    uint8_t device_id;
    uint8_t vehicle_state;
    uint8_t flags;
    uint32_t uptime_s;
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
    int32_t cmd_param;
} NCOM_Payload_COMMAND_t;
#define NCOM_LEN_COMMAND 5

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

// ID 9: ID 9 is free for a message.
typedef struct __attribute__((packed)) {
} NCOM_Payload_UNUSED_t;
#define NCOM_LEN_UNUSED 0

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
size_t ncom_pack_unused(uint8_t *buf, const NCOM_Payload_UNUSED_t *msg);
size_t ncom_unpack_unused(const uint8_t *buf, NCOM_Payload_UNUSED_t *msg);

#endif /* INC_NCOM_PROTOCOL_H */
