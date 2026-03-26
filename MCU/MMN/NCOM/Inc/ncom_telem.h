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
 *                ncom_telem.h
 *   NCOM Telemetry Stream Module Header File
 * ############################################# */
#ifndef INC_NCOM_TELEM_H_
#define INC_NCOM_TELEM_H_

#include "ncom_protocol.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdint.h>

#define NCOM_TELEM_BASE_TICK_MS 10
#define NCOM_TELEM_STREAM_COUNT 8

typedef struct {
  uint8_t msgId;
  uint16_t interval_ms;
  TickType_t lastSent;
} NCOM_Telem_Stream_t;

void NCOM_Telem_Init(void);
void NCOM_Telem_SetRate(uint8_t msgId, uint16_t interval_ms);
void NCOM_Telem_BuildAndSend(uint8_t msgId);
void NCOM_Telem_ServiceAll(void);

extern NCOM_Telem_Stream_t telemStreams[NCOM_TELEM_STREAM_COUNT];

#endif /* INC_NCOM_TELEM_H_ */
