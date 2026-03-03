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
 * NCOM C Source File
 * Auto-generated on: 04.03.2026 02:26:18
 * Protocol Version: 0.1
 */

#include "ncom_protocol.h"
#include <string.h>

size_t ncom_pack_heartbeat(uint8_t* buf, const NCOM_Payload_HEARTBEAT_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_HEARTBEAT_t));
    return sizeof(NCOM_Payload_HEARTBEAT_t);
}

size_t ncom_unpack_heartbeat(const uint8_t* buf, NCOM_Payload_HEARTBEAT_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_HEARTBEAT_t));
    return sizeof(NCOM_Payload_HEARTBEAT_t);
}

size_t ncom_pack_sys_status(uint8_t* buf, const NCOM_Payload_SYS_STATUS_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_SYS_STATUS_t));
    return sizeof(NCOM_Payload_SYS_STATUS_t);
}

size_t ncom_unpack_sys_status(const uint8_t* buf, NCOM_Payload_SYS_STATUS_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_SYS_STATUS_t));
    return sizeof(NCOM_Payload_SYS_STATUS_t);
}

size_t ncom_pack_command(uint8_t* buf, const NCOM_Payload_COMMAND_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_COMMAND_t));
    return sizeof(NCOM_Payload_COMMAND_t);
}

size_t ncom_unpack_command(const uint8_t* buf, NCOM_Payload_COMMAND_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_COMMAND_t));
    return sizeof(NCOM_Payload_COMMAND_t);
}

size_t ncom_pack_attitude(uint8_t* buf, const NCOM_Payload_ATTITUDE_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_ATTITUDE_t));
    return sizeof(NCOM_Payload_ATTITUDE_t);
}

size_t ncom_unpack_attitude(const uint8_t* buf, NCOM_Payload_ATTITUDE_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_ATTITUDE_t));
    return sizeof(NCOM_Payload_ATTITUDE_t);
}

size_t ncom_pack_rotation_rates(uint8_t* buf, const NCOM_Payload_ROTATION_RATES_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_ROTATION_RATES_t));
    return sizeof(NCOM_Payload_ROTATION_RATES_t);
}

size_t ncom_unpack_rotation_rates(const uint8_t* buf, NCOM_Payload_ROTATION_RATES_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_ROTATION_RATES_t));
    return sizeof(NCOM_Payload_ROTATION_RATES_t);
}

size_t ncom_pack_linear_velocity(uint8_t* buf, const NCOM_Payload_LINEAR_VELOCITY_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_LINEAR_VELOCITY_t));
    return sizeof(NCOM_Payload_LINEAR_VELOCITY_t);
}

size_t ncom_unpack_linear_velocity(const uint8_t* buf, NCOM_Payload_LINEAR_VELOCITY_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_LINEAR_VELOCITY_t));
    return sizeof(NCOM_Payload_LINEAR_VELOCITY_t);
}

size_t ncom_pack_depth(uint8_t* buf, const NCOM_Payload_DEPTH_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_DEPTH_t));
    return sizeof(NCOM_Payload_DEPTH_t);
}

size_t ncom_unpack_depth(const uint8_t* buf, NCOM_Payload_DEPTH_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_DEPTH_t));
    return sizeof(NCOM_Payload_DEPTH_t);
}

size_t ncom_pack_mode_status(uint8_t* buf, const NCOM_Payload_MODE_STATUS_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_MODE_STATUS_t));
    return sizeof(NCOM_Payload_MODE_STATUS_t);
}

size_t ncom_unpack_mode_status(const uint8_t* buf, NCOM_Payload_MODE_STATUS_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_MODE_STATUS_t));
    return sizeof(NCOM_Payload_MODE_STATUS_t);
}

size_t ncom_pack_targets(uint8_t* buf, const NCOM_Payload_TARGETS_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_TARGETS_t));
    return sizeof(NCOM_Payload_TARGETS_t);
}

size_t ncom_unpack_targets(const uint8_t* buf, NCOM_Payload_TARGETS_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_TARGETS_t));
    return sizeof(NCOM_Payload_TARGETS_t);
}

size_t ncom_pack_acknowledgement(uint8_t* buf, const NCOM_Payload_ACKNOWLEDGEMENT_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_ACKNOWLEDGEMENT_t));
    return sizeof(NCOM_Payload_ACKNOWLEDGEMENT_t);
}

size_t ncom_unpack_acknowledgement(const uint8_t* buf, NCOM_Payload_ACKNOWLEDGEMENT_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_ACKNOWLEDGEMENT_t));
    return sizeof(NCOM_Payload_ACKNOWLEDGEMENT_t);
}

size_t ncom_pack_config_req(uint8_t* buf, const NCOM_Payload_CONFIG_REQ_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_CONFIG_REQ_t));
    return sizeof(NCOM_Payload_CONFIG_REQ_t);
}

size_t ncom_unpack_config_req(const uint8_t* buf, NCOM_Payload_CONFIG_REQ_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_CONFIG_REQ_t));
    return sizeof(NCOM_Payload_CONFIG_REQ_t);
}

size_t ncom_pack_config_resp(uint8_t* buf, const NCOM_Payload_CONFIG_RESP_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_CONFIG_RESP_t));
    return sizeof(NCOM_Payload_CONFIG_RESP_t);
}

size_t ncom_unpack_config_resp(const uint8_t* buf, NCOM_Payload_CONFIG_RESP_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_CONFIG_RESP_t));
    return sizeof(NCOM_Payload_CONFIG_RESP_t);
}

size_t ncom_pack_config_set_mcu(uint8_t* buf, const NCOM_Payload_CONFIG_SET_MCU_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_CONFIG_SET_MCU_t));
    return sizeof(NCOM_Payload_CONFIG_SET_MCU_t);
}

size_t ncom_unpack_config_set_mcu(const uint8_t* buf, NCOM_Payload_CONFIG_SET_MCU_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_CONFIG_SET_MCU_t));
    return sizeof(NCOM_Payload_CONFIG_SET_MCU_t);
}

size_t ncom_pack_config_set_freertos(uint8_t* buf, const NCOM_Payload_CONFIG_SET_FREERTOS_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_CONFIG_SET_FREERTOS_t));
    return sizeof(NCOM_Payload_CONFIG_SET_FREERTOS_t);
}

size_t ncom_unpack_config_set_freertos(const uint8_t* buf, NCOM_Payload_CONFIG_SET_FREERTOS_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_CONFIG_SET_FREERTOS_t));
    return sizeof(NCOM_Payload_CONFIG_SET_FREERTOS_t);
}

size_t ncom_pack_config_set_sensor(uint8_t* buf, const NCOM_Payload_CONFIG_SET_SENSOR_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_CONFIG_SET_SENSOR_t));
    return sizeof(NCOM_Payload_CONFIG_SET_SENSOR_t);
}

size_t ncom_unpack_config_set_sensor(const uint8_t* buf, NCOM_Payload_CONFIG_SET_SENSOR_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_CONFIG_SET_SENSOR_t));
    return sizeof(NCOM_Payload_CONFIG_SET_SENSOR_t);
}

size_t ncom_pack_config_set_actuator(uint8_t* buf, const NCOM_Payload_CONFIG_SET_ACTUATOR_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_CONFIG_SET_ACTUATOR_t));
    return sizeof(NCOM_Payload_CONFIG_SET_ACTUATOR_t);
}

size_t ncom_unpack_config_set_actuator(const uint8_t* buf, NCOM_Payload_CONFIG_SET_ACTUATOR_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_CONFIG_SET_ACTUATOR_t));
    return sizeof(NCOM_Payload_CONFIG_SET_ACTUATOR_t);
}

size_t ncom_pack_config_set_pid(uint8_t* buf, const NCOM_Payload_CONFIG_SET_PID_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_CONFIG_SET_PID_t));
    return sizeof(NCOM_Payload_CONFIG_SET_PID_t);
}

size_t ncom_unpack_config_set_pid(const uint8_t* buf, NCOM_Payload_CONFIG_SET_PID_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_CONFIG_SET_PID_t));
    return sizeof(NCOM_Payload_CONFIG_SET_PID_t);
}

size_t ncom_pack_config_set_ekf(uint8_t* buf, const NCOM_Payload_CONFIG_SET_EKF_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_CONFIG_SET_EKF_t));
    return sizeof(NCOM_Payload_CONFIG_SET_EKF_t);
}

size_t ncom_unpack_config_set_ekf(const uint8_t* buf, NCOM_Payload_CONFIG_SET_EKF_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_CONFIG_SET_EKF_t));
    return sizeof(NCOM_Payload_CONFIG_SET_EKF_t);
}

size_t ncom_pack_config_req_startup(uint8_t* buf, const NCOM_Payload_CONFIG_REQ_STARTUP_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_CONFIG_REQ_STARTUP_t));
    return sizeof(NCOM_Payload_CONFIG_REQ_STARTUP_t);
}

size_t ncom_unpack_config_req_startup(const uint8_t* buf, NCOM_Payload_CONFIG_REQ_STARTUP_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_CONFIG_REQ_STARTUP_t));
    return sizeof(NCOM_Payload_CONFIG_REQ_STARTUP_t);
}

size_t ncom_pack_config_set_startup(uint8_t* buf, const NCOM_Payload_CONFIG_SET_STARTUP_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_CONFIG_SET_STARTUP_t));
    return sizeof(NCOM_Payload_CONFIG_SET_STARTUP_t);
}

size_t ncom_unpack_config_set_startup(const uint8_t* buf, NCOM_Payload_CONFIG_SET_STARTUP_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_CONFIG_SET_STARTUP_t));
    return sizeof(NCOM_Payload_CONFIG_SET_STARTUP_t);
}

size_t ncom_pack_init_error(uint8_t* buf, const NCOM_Payload_INIT_ERROR_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_INIT_ERROR_t));
    return sizeof(NCOM_Payload_INIT_ERROR_t);
}

size_t ncom_unpack_init_error(const uint8_t* buf, NCOM_Payload_INIT_ERROR_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_INIT_ERROR_t));
    return sizeof(NCOM_Payload_INIT_ERROR_t);
}

size_t ncom_pack_dying_gasp(uint8_t* buf, const NCOM_Payload_DYING_GASP_t* msg) {
    memcpy(buf, msg, sizeof(NCOM_Payload_DYING_GASP_t));
    return sizeof(NCOM_Payload_DYING_GASP_t);
}

size_t ncom_unpack_dying_gasp(const uint8_t* buf, NCOM_Payload_DYING_GASP_t* msg) {
    memcpy(msg, buf, sizeof(NCOM_Payload_DYING_GASP_t));
    return sizeof(NCOM_Payload_DYING_GASP_t);
}

