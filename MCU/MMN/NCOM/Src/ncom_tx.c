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
 *                  ncom_tx.c
 * NCOM MCU-Side Transmission Module Source File
 * ############################################# */

/* #############################################
 * 					         Includes
 * ############################################# */
#include "ncom_tx.h"
#include "usbd_cdc_if.h"
#include <string.h>

#include "FreeRTOS.h"
#include "semphr.h"

/* #############################################
 * 				          Variables
 * ############################################# */
static SemaphoreHandle_t xTxMutex = NULL;

/* #############################################
 * 		      Software CRC16-CCITT (0x1021)
 * ############################################# */

/*
 *	ncom_crc16_sw
 *	Computes CRC16-CCITT (polynomial 0x1021, init 0xFFFF) in software.
 *	This avoids sharing the hardware CRC peripheral with the RX parser.
 *
 *	Parameters: Data buffer, Length in bytes
 *	Returns: CRC16 value
 */
static uint16_t ncom_crc16_sw(const uint8_t *data, size_t len) {
  uint16_t crc = 0xFFFF;
  for (size_t i = 0; i < len; i++) {
    crc ^= (uint16_t)data[i] << 8;
    for (uint8_t bit = 0; bit < 8; bit++) {
      if (crc & 0x8000)
        crc = (crc << 1) ^ 0x1021;
      else
        crc <<= 1;
    }
  }
  return crc;
}

/* #############################################
 * 			        Function Definitions
 * ############################################# */

/*
 *	NCOM_TX_Init
 *	Creates the TX mutex. Must be called before any SendPacket calls.
 *
 *	Parameters: -
 *	Returns: -
 */
void NCOM_TX_Init(void) {
  xTxMutex = xSemaphoreCreateMutex();
  configASSERT(xTxMutex != NULL);
}

/*
 *	ncom_tx_build_and_send
 *	Internal helper: builds the frame and transmits via USB CDC.
 *	Called by both safe (mutex) and unsafe (no mutex) variants.
 *
 *	Parameters: Message ID, Message Payload, Payload Size in Bytes
 *	Returns: Sequence number on success, -1 on failure
 */
static int16_t ncom_tx_build_and_send(uint8_t msgId, const void *payload,
                                      uint8_t len) {

  static uint8_t tx_seq;
  uint8_t seq = tx_seq++;
  static uint8_t txBuf[NCOM_MAX_PAYLOAD_LEN + NCOM_OVERHEAD_LEN];

  txBuf[0] = NCOM_SYNC_BYTE_1;
  txBuf[1] = NCOM_SYNC_BYTE_2;
  txBuf[2] = seq;
  txBuf[3] = msgId;
  txBuf[4] = len;

  if (len > 0 && payload != NULL)
    memcpy(&txBuf[NCOM_HEADER_LEN], payload, len);

  uint16_t crc16 = ncom_crc16_sw(&txBuf[NCOM_SYNCBYTE_COUNT],
                                 len + NCOM_HEADER_LEN - NCOM_SYNCBYTE_COUNT);

  txBuf[len + NCOM_HEADER_LEN] = (uint8_t)(crc16 & 0xFF);
  txBuf[len + NCOM_HEADER_LEN + 1] = (uint8_t)((crc16 >> 8) & 0xFF);

  uint8_t result = CDC_Transmit_FS(txBuf, len + NCOM_OVERHEAD_LEN);
  if (result != USBD_OK)
    return -1;

  return (int16_t)seq;
}

/*
 *	NCOM_TX_SendPacket
 *	Thread-safe packet transmission via USB OTG FS.
 *	Acquires the TX mutex before building and sending the frame.
 *
 *	Parameters: Message ID, Message Payload, Payload Size in Bytes
 *	Returns: Sequence number (0-255) on success, -1 on failure
 */
int16_t NCOM_TX_SendPacket(uint8_t msgId, const void *payload, uint8_t len) {

  if (len > NCOM_MAX_PAYLOAD_LEN)
    return -1;

  if (xTxMutex == NULL)
    return -1;

  if (xSemaphoreTake(xTxMutex, pdMS_TO_TICKS(10)) != pdTRUE)
    return -1;

  int16_t result = ncom_tx_build_and_send(msgId, payload, len);

  xSemaphoreGive(xTxMutex);

  return result;
}

/*
 *	NCOM_TX_SendPacketUnsafe
 *	Non-thread-safe packet transmission. For use ONLY in contexts where
 *	the RTOS scheduler is not running or all other tasks are dead
 *	(e.g., dying gasp before IWDG reset).
 *
 *	Parameters: Message ID, Message Payload, Payload Size in Bytes
 *	Returns: Sequence number (0-255) on success, -1 on failure
 */
int16_t NCOM_TX_SendPacketUnsafe(uint8_t msgId, const void *payload,
                                 uint8_t len) {

  if (len > NCOM_MAX_PAYLOAD_LEN)
    return -1;

  return ncom_tx_build_and_send(msgId, payload, len);
}

/* #############################################
 * #############################################
 * ############################################# */
