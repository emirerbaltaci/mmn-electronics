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

/* #############################################
 *                ncom_telem.c
 *   NCOM Telemetry Stream Module Source File
 * ############################################# */

#include "ncom_telem.h"
#include "ncom_tx.h"
#include "auvflags.h"
#include "bar30.h"
#include "hydrophone.h"
#include "semphr.h"

/* Externs from main.c */
extern volatile AUV_VehicleState_t vehicleStatus;
extern volatile uint8_t missedHostHbAcks;
extern float cpuLoad;
extern uint16_t mvBatVoltage;
extern float ekfState[12];
extern SemaphoreHandle_t xStateMutex;
extern BAR30_Handler_t bar;
extern Hydro_Handler_t hydro;

NCOM_Telem_Stream_t telemStreams[NCOM_TELEM_STREAM_COUNT] = {
    {NCOM_MSG_HEARTBEAT, 1000, 0},
    {NCOM_MSG_SYS_STATUS, 1000, 0},
    {NCOM_MSG_ATTITUDE, 0, 0},
    {NCOM_MSG_ROTATION_RATES, 0, 0},
    {NCOM_MSG_LINEAR_VELOCITY, 0, 0},
    {NCOM_MSG_DEPTH, 0, 0},
    {NCOM_MSG_MODE_STATUS, 0, 0},
    {NCOM_MSG_TARGETS, 0, 0},
};

/*
 * NCOM_Telem_Init
 * Resets all lastSent timestamps to current tick.
 */
void NCOM_Telem_Init(void) {
  TickType_t xNow = xTaskGetTickCount();
  for (int i = 0; i < NCOM_TELEM_STREAM_COUNT; i++)
    telemStreams[i].lastSent = xNow;
}

/*
 * NCOM_Telem_SetRate
 * Sets the interval for a telemetry stream. interval_ms=0 disables it.
 */
void NCOM_Telem_SetRate(uint8_t msgId, uint16_t interval_ms) {
  for (int i = 0; i < NCOM_TELEM_STREAM_COUNT; i++) {
    if (telemStreams[i].msgId == msgId) {
      telemStreams[i].interval_ms = interval_ms;
      telemStreams[i].lastSent = xTaskGetTickCount();
      return;
    }
  }
}

/*
 * NCOM_Telem_BuildAndSend
 * Builds and transmits a telemetry packet for the given NCOM message ID.
 */
void NCOM_Telem_BuildAndSend(uint8_t msgId) {
  switch (msgId) {

  case NCOM_MSG_HEARTBEAT: {
    if (vehicleStatus == AUV_ARMED) missedHostHbAcks++;
    else missedHostHbAcks = 0;
    NCOM_Payload_HEARTBEAT_t hb;
    hb.device_id = 1;
    hb.vehicle_state = vehicleStatus;
    hb.flags = (uint8_t)(configurableFlags & 0xFF);
    hb.uptime_ms = pdTICKS_TO_MS(xTaskGetTickCount());
    uint8_t buf[NCOM_LEN_HEARTBEAT];
    ncom_pack_heartbeat(buf, &hb);
    NCOM_TX_SendPacket(NCOM_MSG_HEARTBEAT, buf, NCOM_LEN_HEARTBEAT);
    break;
  }

  case NCOM_MSG_SYS_STATUS: {
    NCOM_Payload_SYS_STATUS_t ss;
    ss.cpu_load_perc = (uint8_t)cpuLoad;
    ss.bat_voltage_mv = mvBatVoltage;
    ss.mcu_temp_c = 0.0f; // TODO: ADC temperature reading
    uint8_t buf[NCOM_LEN_SYS_STATUS];
    ncom_pack_sys_status(buf, &ss);
    NCOM_TX_SendPacket(NCOM_MSG_SYS_STATUS, buf, NCOM_LEN_SYS_STATUS);
    break;
  }

  case NCOM_MSG_ATTITUDE: {
    NCOM_Payload_ATTITUDE_t att;
    if (xSemaphoreTake(xStateMutex, 0) == pdTRUE) {
      att.roll_deg = ekfState[3] * 57.29577951f;
      att.pitch_deg = ekfState[4] * 57.29577951f;
      att.yaw_deg = ekfState[5] * 57.29577951f;
      att.validity = 1;
      xSemaphoreGive(xStateMutex);
    } else return;
    uint8_t buf[NCOM_LEN_ATTITUDE];
    ncom_pack_attitude(buf, &att);
    NCOM_TX_SendPacket(NCOM_MSG_ATTITUDE, buf, NCOM_LEN_ATTITUDE);
    break;
  }

  case NCOM_MSG_ROTATION_RATES: {
    NCOM_Payload_ROTATION_RATES_t rr;
    if (xSemaphoreTake(xStateMutex, 0) == pdTRUE) {
      rr.p_deg_s = ekfState[9] * 57.29577951f;
      rr.q_deg_s = ekfState[10] * 57.29577951f;
      rr.r_deg_s = ekfState[11] * 57.29577951f;
      xSemaphoreGive(xStateMutex);
    } else return;
    uint8_t buf[NCOM_LEN_ROTATION_RATES];
    ncom_pack_rotation_rates(buf, &rr);
    NCOM_TX_SendPacket(NCOM_MSG_ROTATION_RATES, buf, NCOM_LEN_ROTATION_RATES);
    break;
  }

  case NCOM_MSG_LINEAR_VELOCITY: {
    NCOM_Payload_LINEAR_VELOCITY_t lv;
    if (xSemaphoreTake(xStateMutex, 0) == pdTRUE) {
      lv.surge_mm_s = ekfState[6] * 1000.0f;
      lv.sway_mm_s = ekfState[7] * 1000.0f;
      lv.heave_mm_s = ekfState[8] * 1000.0f;
      xSemaphoreGive(xStateMutex);
    } else return;
    uint8_t buf[NCOM_LEN_LINEAR_VELOCITY];
    ncom_pack_linear_velocity(buf, &lv);
    NCOM_TX_SendPacket(NCOM_MSG_LINEAR_VELOCITY, buf, NCOM_LEN_LINEAR_VELOCITY);
    break;
  }

  case NCOM_MSG_DEPTH: {
    NCOM_Payload_DEPTH_t dp;
    if (xSemaphoreTake(xStateMutex, 0) == pdTRUE) {
      dp.depth_mm = (int32_t)(bar.depth * 1000.0f);
      dp.depth_rate_mm_s = (int32_t)(ekfState[8] * 1000.0f);
      dp.depth_hold_is_active = 0; // TODO
      xSemaphoreGive(xStateMutex);
    } else return;
    uint8_t buf[NCOM_LEN_DEPTH];
    ncom_pack_depth(buf, &dp);
    NCOM_TX_SendPacket(NCOM_MSG_DEPTH, buf, NCOM_LEN_DEPTH);
    break;
  }

  case NCOM_MSG_HYDROPHONE_STATUS: {
    NCOM_Payload_HYDROPHONE_STATUS_t hs;
    hs.dominant_freq_hz = hydro.dominant_freq_hz;
    hs.peak_magnitude   = hydro.peak_magnitude;
    hs.status_flags     = 0;
    if (hydro.signal_detected)
      hs.status_flags |= NCOM_FLAG_HYDROPHONE_STATUS_SIGNAL_DETECTED;
    if (hydro.clipping)
      hs.status_flags |= NCOM_FLAG_HYDROPHONE_STATUS_CLIPPING;
    uint8_t buf[NCOM_LEN_HYDROPHONE_STATUS];
    ncom_pack_hydrophone_status(buf, &hs);
    NCOM_TX_SendPacket(NCOM_MSG_HYDROPHONE_STATUS, buf, NCOM_LEN_HYDROPHONE_STATUS);
    break;
  }

  default:
    break;
  }
}

/*
 * NCOM_Telem_ServiceAll
 * Checks all streams and sends any that are due.
 */
void NCOM_Telem_ServiceAll(void) {
  TickType_t xNow = xTaskGetTickCount();
  for (int i = 0; i < NCOM_TELEM_STREAM_COUNT; i++) {
    if (telemStreams[i].interval_ms == 0) continue;
    if ((xNow - telemStreams[i].lastSent) >= pdMS_TO_TICKS(telemStreams[i].interval_ms)) {
      telemStreams[i].lastSent = xNow;
      NCOM_Telem_BuildAndSend(telemStreams[i].msgId);
    }
  }
}
