/*
 * MIT License
 *
 * Copyright (c) 2026 Emir Erbaltacı
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
 * Auto-generated on: 06.02.2026 00:59:32
 * Protocol Version: 0.1
 */

#ifndef INC_NCOM_DEF_H_
#define INC_NCOM_DEF_H_

#include <stdint.h>

#define NCOM_SYNC_BYTE 0xA5

typedef enum {
    NCOM_MSG_HEARTBEAT = 0,
    NCOM_MSG_SYS_STATUS = 1,
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
#define NCOM_HEARTBEAT_VEHICLE_STATE_MCU_HARDFAULT 255

#define NCOM_FLAG_HEARTBEAT_LEAK_DETECTED (1 << 0)
#define NCOM_FLAG_HEARTBEAT_BATTERY_LOW (1 << 1)
#define NCOM_FLAG_HEARTBEAT_INIT_ERR (1 << 2)
#define NCOM_FLAG_HEARTBEAT_NAV_ERR (1 << 3)
#define NCOM_FLAG_HEARTBEAT_BAR30_ERR (1 << 4)
#define NCOM_FLAG_HEARTBEAT_PERIPHERAL_ERR (1 << 5)
#define NCOM_FLAG_HEARTBEAT_RTOS_TASK_ERR (1 << 6)
#define NCOM_FLAG_HEARTBEAT_MISSION_ABORTED (1 << 7)

// ID 0: Sent with a freq. of 1Hz
typedef struct __attribute__((packed)) {
    uint8_t device_id;
    uint8_t vehicle_state;
    uint8_t flags;
    uint32_t uptime_seconds;
} NCOM_Payload_HEARTBEAT_t;
#define NCOM_LEN_HEARTBEAT 7

// ID 1: System Status
typedef struct __attribute__((packed)) {
    uint8_t cpu_load_perc;
    uint16_t bat_voltage_mv;
    uint16_t mcu_temp_raw;
} NCOM_Payload_SYS_STATUS_t;
#define NCOM_LEN_SYS_STATUS 5

#endif /* INC_NCOM_PROTOCOL_H */
