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

#include "ncom_handlers.h"
#include "ncom_tx.h"
#include <string.h>

/*
 * Handler return value = ACKNOWLEDGEMENT response code:
 *   NCOM_ACKNOWLEDGEMENT_RESPONSE_ACK               (1)   - Success
 *   NCOM_ACKNOWLEDGEMENT_RESPONSE_NACK              (0)   - Rejected
 *   NCOM_ACKNOWLEDGEMENT_RESPONSE_INVALID_PARAMETER (254) - Bad parameter
 *   NCOM_ACKNOWLEDGEMENT_RESPONSE_UNKNOWN_ERROR     (255) - Internal error
 */
typedef uint8_t (*NCOM_Handler)(NCOM_RX_t *rx);

/*
 * Helper: sends an ACKNOWLEDGEMENT packet for the given message.
 */
static void NCOM_Handlers_SendAck(uint8_t msgId, uint8_t seq,
                                  uint8_t response) {
  NCOM_Payload_ACKNOWLEDGEMENT_t ack;
  ack.requested_msg_id = msgId;
  ack.requested_seq = seq;
  ack.response = response;

  uint8_t buf[NCOM_LEN_ACKNOWLEDGEMENT];
  ncom_pack_acknowledgement(buf, &ack);
  NCOM_TX_SendPacket(NCOM_MSG_ACKNOWLEDGEMENT, buf, NCOM_LEN_ACKNOWLEDGEMENT);
}

/*
 * Helper: reinterpret int32_t config_value as float32.
 * Used when data_type == FLOAT32 (type-punning via memcpy).
 */
static inline float NCOM_ConfigValue_AsFloat(int32_t raw) {
  float f;
  memcpy(&f, &raw, sizeof(float));
  return f;
}

/* ============================================================================
 * Startup Configuration Handler
 * Called during handshake phase (before scheduler), not via dispatch table.
 * ========================================================================= */
bool NCOM_Handlers_Config_Set_Startup(NCOM_RX_t *rx, AUV_Config_t *auvCfg) {

  NCOM_Payload_CONFIG_SET_STARTUP_t msg;
  ncom_unpack_config_set_startup(rx->parser.payloadBuf, &msg);

  /* TODO: Populate auvCfg from startup message once payload is defined.
   * CONFIG_SET_STARTUP is currently an empty payload (0 bytes). */

  return true;
}

/* ============================================================================
 * COMMAND Handler (ID 2)
 * Payload: { cmd_id, cmd_param0..5 }
 * ========================================================================= */
static uint8_t NCOM_Handlers_Command(NCOM_RX_t *rx) {

  NCOM_Payload_COMMAND_t msg;
  ncom_unpack_command(rx->parser.payloadBuf, &msg);

  switch (msg.cmd_id) {
  case NCOM_COMMAND_CMD_ID_DISARM:
  case NCOM_COMMAND_CMD_ID_PREARM:
  case NCOM_COMMAND_CMD_ID_ARM:
  case NCOM_COMMAND_CMD_ID_START_MISSION:
  case NCOM_COMMAND_CMD_ID_ABORT_MISSION:
  case NCOM_COMMAND_CMD_ID_CLEAR_FLAGS:
  case NCOM_COMMAND_CMD_ID_SET_MODE:
  case NCOM_COMMAND_CMD_ID_SET_TARGET_DEPTH_MM:
  case NCOM_COMMAND_CMD_ID_SET_TARGET_ROLL_DEG:
  case NCOM_COMMAND_CMD_ID_SET_TARGET_PITCH_DEG:
  case NCOM_COMMAND_CMD_ID_SET_TARGET_YAW_DEG:
  case NCOM_COMMAND_CMD_ID_SET_TARGET_SURGE_MM_S:
  case NCOM_COMMAND_CMD_ID_SET_TARGET_SWAY_MM_S:
  case NCOM_COMMAND_CMD_ID_SET_TARGET_HEAVE_MM_S:
  case NCOM_COMMAND_CMD_ID_SET_LIGHTS:
  case NCOM_COMMAND_CMD_ID_REBOOT_MCU:
  case NCOM_COMMAND_CMD_ID_CALIBRATE_DEPTH_0:
  case NCOM_COMMAND_CMD_ID_CALIBRATE_AXES_0:
  case NCOM_COMMAND_CMD_ID_SET_HEARTBEAT_RATE:
  case NCOM_COMMAND_CMD_ID_EMERGENCY:
    /* TODO: Execute command via params msg.cmd_param0..5 */
    break;
  default:
    return NCOM_ACKNOWLEDGEMENT_RESPONSE_INVALID_PARAMETER;
  }

  return NCOM_ACKNOWLEDGEMENT_RESPONSE_ACK;
}

/* ============================================================================
 * CONFIG_REQ Handler (ID 40)
 * Payload: { config_id }
 * Looks up the requested config and sends back a CONFIG_RESP packet.
 * ========================================================================= */
static uint8_t NCOM_Handlers_Config_Req(NCOM_RX_t *rx) {

  NCOM_Payload_CONFIG_REQ_t msg;
  ncom_unpack_config_req(rx->parser.payloadBuf, &msg);

  NCOM_Payload_CONFIG_RESP_t resp;
  resp.config_id = msg.config_id;
  resp.data_type = NCOM_CONFIG_RESP_DATA_TYPE_FLOAT32;

  float value;
  switch (msg.config_id) {
  /* PID */
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_ROLL_P:
    value = auvConfig.pid.roll.p;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_ROLL_I:
    value = auvConfig.pid.roll.i;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_ROLL_D:
    value = auvConfig.pid.roll.d;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_PITCH_P:
    value = auvConfig.pid.pitch.p;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_PITCH_I:
    value = auvConfig.pid.pitch.i;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_PITCH_D:
    value = auvConfig.pid.pitch.d;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_YAW_P:
    value = auvConfig.pid.yaw.p;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_YAW_I:
    value = auvConfig.pid.yaw.i;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_YAW_D:
    value = auvConfig.pid.yaw.d;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_DEPTH_P:
    value = auvConfig.pid.depth.p;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_DEPTH_I:
    value = auvConfig.pid.depth.i;
    break;
  case NCOM_CONFIG_REQ_CONFIG_ID_PID_DEPTH_D:
    value = auvConfig.pid.depth.d;
    break;
  /* Task */
  case NCOM_CONFIG_REQ_CONFIG_ID_TASK_RATE_TELEM:
    resp.data_type = NCOM_CONFIG_RESP_DATA_TYPE_UINT32;
    memcpy(&resp.config_value, &auvConfig.task.ncom_sleep_ms, sizeof(uint32_t));
    goto send;
  case NCOM_CONFIG_REQ_CONFIG_ID_TASK_RATE_CTRL:
    resp.data_type = NCOM_CONFIG_RESP_DATA_TYPE_UINT32;
    memcpy(&resp.config_value, &auvConfig.task.control_sleep_ms,
           sizeof(uint32_t));
    goto send;
  default:
    return NCOM_ACKNOWLEDGEMENT_RESPONSE_INVALID_PARAMETER;
  }

  /* Float path: type-pun float → int32 */
  memcpy(&resp.config_value, &value, sizeof(float));

send:;
  uint8_t buf[NCOM_LEN_CONFIG_RESP];
  ncom_pack_config_resp(buf, &resp);
  NCOM_TX_SendPacket(NCOM_MSG_CONFIG_RESP, buf, NCOM_LEN_CONFIG_RESP);

  return NCOM_ACKNOWLEDGEMENT_RESPONSE_ACK;
}

/* ============================================================================
 * CONFIG_SET_MCU Handler (ID 42)
 * Payload: { config_id, data_type, config_value, save_to_flash }
 * ========================================================================= */
static uint8_t NCOM_Handlers_Config_Set_Mcu(NCOM_RX_t *rx) {

  NCOM_Payload_CONFIG_SET_MCU_t msg;
  ncom_unpack_config_set_mcu(rx->parser.payloadBuf, &msg);

  switch (msg.config_id) {
  case NCOM_CONFIG_SET_MCU_CONFIG_ID_TELEM_RATE_HZ:
    /* TODO: Apply telemetry rate change */
    break;
  default:
    return NCOM_ACKNOWLEDGEMENT_RESPONSE_INVALID_PARAMETER;
  }

  return NCOM_ACKNOWLEDGEMENT_RESPONSE_ACK;
}

/* ============================================================================
 * CONFIG_SET_FREERTOS Handler (ID 43)
 * Payload: { config_id, data_type, config_value, save_to_flash }
 * ========================================================================= */
static uint8_t NCOM_Handlers_Config_Set_Freertos(NCOM_RX_t *rx) {

  NCOM_Payload_CONFIG_SET_FREERTOS_t msg;
  ncom_unpack_config_set_freertos(rx->parser.payloadBuf, &msg);

  switch (msg.config_id) {
  case NCOM_CONFIG_SET_FREERTOS_CONFIG_ID_TASK_RATE_TELEM:
    auvConfig.task.ncom_sleep_ms = (uint32_t)msg.config_value;
    break;
  case NCOM_CONFIG_SET_FREERTOS_CONFIG_ID_TASK_RATE_CTRL:
    auvConfig.task.control_sleep_ms = (uint32_t)msg.config_value;
    break;
  default:
    return NCOM_ACKNOWLEDGEMENT_RESPONSE_INVALID_PARAMETER;
  }

  return NCOM_ACKNOWLEDGEMENT_RESPONSE_ACK;
}

/* ============================================================================
 * CONFIG_SET_SENSOR Handler (ID 44)
 * Payload: { config_id, data_type, config_value, save_to_flash }
 * ========================================================================= */
static uint8_t NCOM_Handlers_Config_Set_Sensor(NCOM_RX_t *rx) {

  NCOM_Payload_CONFIG_SET_SENSOR_t msg;
  ncom_unpack_config_set_sensor(rx->parser.payloadBuf, &msg);

  switch (msg.config_id) {
  case NCOM_CONFIG_SET_SENSOR_CONFIG_ID_IMU_RATE_HZ:
  case NCOM_CONFIG_SET_SENSOR_CONFIG_ID_BARO_RATE_HZ:
  case NCOM_CONFIG_SET_SENSOR_CONFIG_ID_MAG_RATE_HZ:
    /* TODO: Apply sensor rate change */
    break;
  default:
    return NCOM_ACKNOWLEDGEMENT_RESPONSE_INVALID_PARAMETER;
  }

  return NCOM_ACKNOWLEDGEMENT_RESPONSE_ACK;
}

/* ============================================================================
 * CONFIG_SET_ACTUATOR Handler (ID 45)
 * Payload: { config_id, data_type, config_value, save_to_flash }
 * ========================================================================= */
static uint8_t NCOM_Handlers_Config_Set_Actuator(NCOM_RX_t *rx) {

  NCOM_Payload_CONFIG_SET_ACTUATOR_t msg;
  ncom_unpack_config_set_actuator(rx->parser.payloadBuf, &msg);

  switch (msg.config_id) {
  case NCOM_CONFIG_SET_ACTUATOR_CONFIG_ID_PWM_MIN_PULSE:
    auvConfig.thruster.pwm_min = (uint16_t)msg.config_value;
    break;
  case NCOM_CONFIG_SET_ACTUATOR_CONFIG_ID_PWM_MAX_PULSE:
    auvConfig.thruster.pwm_max = (uint16_t)msg.config_value;
    break;
  case NCOM_CONFIG_SET_ACTUATOR_CONFIG_ID_ESC_MODE:
    /* TODO: Apply ESC mode */
    break;
  default:
    return NCOM_ACKNOWLEDGEMENT_RESPONSE_INVALID_PARAMETER;
  }

  return NCOM_ACKNOWLEDGEMENT_RESPONSE_ACK;
}

/* ============================================================================
 * CONFIG_SET_PID Handler (ID 46)
 * Payload: { config_id, data_type, config_value, save_to_flash }
 * config_value is float32 type-punned as int32.
 * ========================================================================= */
static uint8_t NCOM_Handlers_Config_Set_PID(NCOM_RX_t *rx) {

  NCOM_Payload_CONFIG_SET_PID_t msg;
  ncom_unpack_config_set_pid(rx->parser.payloadBuf, &msg);

  float val = NCOM_ConfigValue_AsFloat(msg.config_value);

  switch (msg.config_id) {
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_ROLL_P:
    auvConfig.pid.roll.p = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_ROLL_I:
    auvConfig.pid.roll.i = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_ROLL_D:
    auvConfig.pid.roll.d = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_PITCH_P:
    auvConfig.pid.pitch.p = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_PITCH_I:
    auvConfig.pid.pitch.i = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_PITCH_D:
    auvConfig.pid.pitch.d = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_YAW_P:
    auvConfig.pid.yaw.p = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_YAW_I:
    auvConfig.pid.yaw.i = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_YAW_D:
    auvConfig.pid.yaw.d = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_DEPTH_P:
    auvConfig.pid.depth.p = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_DEPTH_I:
    auvConfig.pid.depth.i = val;
    break;
  case NCOM_CONFIG_SET_PID_CONFIG_ID_PID_DEPTH_D:
    auvConfig.pid.depth.d = val;
    break;
  default:
    return NCOM_ACKNOWLEDGEMENT_RESPONSE_INVALID_PARAMETER;
  }

  return NCOM_ACKNOWLEDGEMENT_RESPONSE_ACK;
}

/* ============================================================================
 * CONFIG_SET_EKF Handler (ID 47)
 * Payload: { config_id, data_type, config_value[9] }
 * config_value is an array of 9 int32 values (float32 type-punned).
 * ========================================================================= */
static uint8_t NCOM_Handlers_Config_Set_EKF(NCOM_RX_t *rx) {

  NCOM_Payload_CONFIG_SET_EKF_t msg;
  ncom_unpack_config_set_ekf(rx->parser.payloadBuf, &msg);

  switch (msg.config_id) {
  case NCOM_CONFIG_SET_EKF_CONFIG_ID_EKF_PROCESS_NOISE:
    auvConfig.ekf.q_vel_noise = NCOM_ConfigValue_AsFloat(msg.config_value[0]);
    auvConfig.ekf.q_att_noise = NCOM_ConfigValue_AsFloat(msg.config_value[1]);
    auvConfig.ekf.q_bg_noise = NCOM_ConfigValue_AsFloat(msg.config_value[2]);
    auvConfig.ekf.q_ba_noise = NCOM_ConfigValue_AsFloat(msg.config_value[3]);
    break;
  case NCOM_CONFIG_SET_EKF_CONFIG_ID_EKF_MEAS_NOISE:
    auvConfig.ekf.r_accel = NCOM_ConfigValue_AsFloat(msg.config_value[0]);
    auvConfig.ekf.r_mag = NCOM_ConfigValue_AsFloat(msg.config_value[1]);
    auvConfig.ekf.r_baro = NCOM_ConfigValue_AsFloat(msg.config_value[2]);
    auvConfig.ekf.r_zupt = NCOM_ConfigValue_AsFloat(msg.config_value[3]);
    break;
  default:
    return NCOM_ACKNOWLEDGEMENT_RESPONSE_INVALID_PARAMETER;
  }

  return NCOM_ACKNOWLEDGEMENT_RESPONSE_ACK;
}

/* ============================================================================
 * Dispatch Table
 * ========================================================================= */
static const NCOM_Handler NCOM_HandlersTable[256]
    __attribute__((section(".ccmram"))) = {
        [NCOM_MSG_COMMAND] = NCOM_Handlers_Command,
        [NCOM_MSG_CONFIG_REQ] = NCOM_Handlers_Config_Req,
        [NCOM_MSG_CONFIG_SET_MCU] = NCOM_Handlers_Config_Set_Mcu,
        [NCOM_MSG_CONFIG_SET_FREERTOS] = NCOM_Handlers_Config_Set_Freertos,
        [NCOM_MSG_CONFIG_SET_SENSOR] = NCOM_Handlers_Config_Set_Sensor,
        [NCOM_MSG_CONFIG_SET_ACTUATOR] = NCOM_Handlers_Config_Set_Actuator,
        [NCOM_MSG_CONFIG_SET_PID] = NCOM_Handlers_Config_Set_PID,
        [NCOM_MSG_CONFIG_SET_EKF] = NCOM_Handlers_Config_Set_EKF};

/* ============================================================================
 * Dispatch & ACK
 * ========================================================================= */
void NCOM_Handlers_Selector(NCOM_RX_t *rx) {

  uint8_t id = rx->parser.msgId;
  uint8_t seq = rx->parser.seq;

  if (NCOM_HandlersTable[id] != NULL) {
    if (!AUV_Config_Lock(50)) {
      NCOM_Handlers_SendAck(id, seq,
                            NCOM_ACKNOWLEDGEMENT_RESPONSE_UNKNOWN_ERROR);
      return;
    }
    uint8_t response = NCOM_HandlersTable[id](rx);
    AUV_Config_Unlock();
    NCOM_Handlers_SendAck(id, seq, response);
  } else {
    rx->stats.unknownIDErrors++;
    NCOM_Handlers_SendAck(id, seq, NCOM_ACKNOWLEDGEMENT_RESPONSE_NACK);
  }
}
