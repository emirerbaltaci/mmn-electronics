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
 * 					 ncom_rx.h
 *   NCOM MCU-Side Reception Module Header File
 * ############################################# */
#ifndef INC_NCOM_RX_H_
#define INC_NCOM_RX_H_

/* #############################################
 * 			 Includes and Definitions
 * ############################################# */
#include "ncom_protocol.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>


#define NCOM_RINGBUFFER_SIZE 2048

/* #############################################
 * 					Enumerations
 * ############################################# */
typedef enum { // Parser States
  NCOM_RX_STATE_SYNC_1,
  NCOM_RX_STATE_SYNC_2,
  NCOM_RX_STATE_SEQ,
  NCOM_RX_STATE_ID,
  NCOM_RX_STATE_LEN,
  NCOM_RX_STATE_PAYLOAD,
  NCOM_RX_STATE_CRC
} NCOM_RX_State_t;

/* #############################################
 * 			   Structure Definitions
 * ############################################# */
typedef struct { // Parser Structure
  NCOM_RX_State_t state;

  uint8_t seq;
  uint8_t msgId;

  uint8_t payloadLen;
  uint8_t payloadBuf[NCOM_MAX_PAYLOAD_LEN];
  uint8_t payloadIndex;

  uint8_t crcBuf[2];
  uint8_t crcIndex;
} NCOM_RX_Parser_t;

/*
 * Single-producer / single-consumer ring buffer.
 * Safety contract:
 *   - head is written ONLY by the USB ISR (CDC_Receive_FS)
 *   - tail is written ONLY by Task_NCOM
 *   - Both are volatile to prevent compiler reordering
 * This is safe on Cortex-M4 because aligned 16-bit writes are atomic.
 * Do NOT add a second writer to either variable.
 */
typedef struct {
  uint8_t buf[NCOM_RINGBUFFER_SIZE];
  volatile uint16_t head;
  volatile uint16_t tail;
  volatile uint32_t droppedBytes;
} NCOM_RingBuffer_t;

typedef struct {
  uint32_t receivedBytes;
  uint32_t validPackets;
  uint32_t crcErrors;
  uint32_t lenErrors;
  uint32_t timeoutErrors;
  uint32_t unknownIDErrors;
  uint32_t invalidParamErrors;
} NCOM_RX_Stats_t;

typedef struct {
  NCOM_RX_Parser_t parser;
  NCOM_RingBuffer_t ringBuffer;
  NCOM_RX_Stats_t stats;
} NCOM_RX_t;

/* #############################################
 * 				Function Prototypes
 * ############################################# */
void NCOM_RX_Init(NCOM_RX_t *rx);
bool NCOM_RX_ParseByte(NCOM_RX_t *rx, uint8_t byte);
void NCOM_RX_RingBuffer_Write(NCOM_RX_t *rx, const uint8_t *data, uint16_t len);
bool NCOM_RX_RingBuffer_Read(NCOM_RX_t *rx, uint8_t *byte);
uint16_t NCOM_RX_RingBuffer_GetSize(NCOM_RX_t *rx);

/* #############################################
 * #############################################
 * ############################################# */
#endif /* INC_NCOM_RX_H_ */
