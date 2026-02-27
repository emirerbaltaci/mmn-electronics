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



/* #############################################
 * 					 ncom_tx.c
 * NCOM MCU-Side Transmission Module Source File
 * ############################################# */



/* #############################################
 * 					 Includes
 * ############################################# */
#include "ncom_tx.h"
#include "usbd_cdc_if.h"
#include "main.h"

#include "FreeRTOS.h"

#ifdef INC_FREERTOS_H
#include "semphr.h"
#endif



/* #############################################
 * 					 Variables
 * ############################################# */
extern CRC_HandleTypeDef hcrc;	// For Hardware CRC Calculation
#ifdef INC_FREERTOS_H
static SemaphoreHandle_t xUSBTxMutex = NULL;
#endif



/* #############################################
 * 			   Function Definitions
 * ############################################# */

/*
 *  NCOM_TX_Init
 *  Creates a mutex if FreeRTOS is enabled.
 *
 *  Parameters: -
 *  Returns: -
 */
void NCOM_TX_Init(void){
#ifdef INC_FREERTOS_H
	if(xUSBTxMutex == NULL) xUSBTxMutex = xSemaphoreCreateMutex();
#endif
}
 
/*
 *	NCOM_TX_SendPacket
 *	Transmits the message via USB OTG FS
 *
 *	Parameters: Message ID, Message Payload, Payload Size in Bytes
 *	Returns: USB Transmit Status
 */
uint8_t NCOM_TX_SendPacket(uint8_t msgId, const void* payload, uint8_t len){
	
	if(len > NCOM_MAX_PAYLOAD_LEN) return USBD_FAIL;
	
	static uint8_t tx_seq = 0;
	static uint8_t txBuf[NCOM_MAX_PAYLOAD_LEN + NCOM_OVERHEAD_LEN];
	
#ifdef INC_FREERTOS_H
	if(xUSBTxMutex != NULL){
		if(xSemaphoreTake(xUSBTxMutex, pdMS_TO_TICKS(10)) != pdTRUE) return USBD_BUSY;
	}
#endif

	txBuf[0] = NCOM_SYNC_BYTE_1;
	txBuf[1] = NCOM_SYNC_BYTE_2;
	txBuf[2] = tx_seq;
	txBuf[3] = msgId;
	txBuf[4] = len;
	
	if(len > 0 && payload != NULL) memcpy(&txBuf[NCOM_HEADER_LEN], payload, len);
	
	__HAL_CRC_DR_RESET(&hcrc);
	for(int i = NCOM_SYNCBYTE_COUNT; i < NCOM_HEADER_LEN; i++) *(uint8_t*)(&hcrc.Instance->DR) = txBuf[i];
	for(int i = 0; i < len; i++) *(uint8_t*)(&hcrc.Instance->DR) = txBuf[NCOM_HEADER_LEN + i];
	uint16_t crc16 = (uint16_t)(hcrc.Instance->DR & 0xFFFF);
	txBuf[len + NCOM_HEADER_LEN] = (uint8_t)(crc16 & 0xFF);
	txBuf[len + NCOM_HEADER_LEN + 1] = (uint8_t)((crc16 >> 8) & 0xFF);
	
	uint8_t status = CDC_Transmit_FS(txBuf, len + NCOM_HEADER_LEN + NCOM_FOOTER_LEN);
	
	if(status == USBD_OK) tx_seq++;
	
#ifdef INC_FREERTOS_H
	if(xUSBTxMutex != NULL) xSemaphoreGive(xUSBTxMutex);
#endif

	return status;
}

#ifdef INC_FREERTOS_H
/*
 *	NCOM_TX_DyingGasp
 *	Transmits a dying message independent of FreeRTOS API
 *
 *	Parameters: Message Payload, Payload Size in Bytes
 *	Returns: USB Transmit Status
 */
uint8_t NCOM_TX_DyingGasp(const void* payload, uint8_t len){

	if(len > NCOM_MAX_PAYLOAD_LEN) return USBD_FAIL;

	static uint8_t tx_seq = 0;
	static uint8_t txBuf[NCOM_MAX_PAYLOAD_LEN + NCOM_OVERHEAD_LEN];

	txBuf[0] = NCOM_SYNC_BYTE_1;
	txBuf[1] = NCOM_SYNC_BYTE_2;
	txBuf[2] = tx_seq;
	txBuf[3] = NCOM_MSG_DYING_GASP;
	txBuf[4] = len;

	if(len > 0 && payload != NULL) memcpy(&txBuf[NCOM_HEADER_LEN], payload, len);

	__HAL_CRC_DR_RESET(&hcrc);
	for(int i = NCOM_SYNCBYTE_COUNT; i < NCOM_HEADER_LEN; i++) *(uint8_t*)(&hcrc.Instance->DR) = txBuf[i];
	for(int i = 0; i < len; i++) *(uint8_t*)(&hcrc.Instance->DR) = txBuf[NCOM_HEADER_LEN + i];
	uint16_t crc16 = (uint16_t)(hcrc.Instance->DR & 0xFFFF);
	txBuf[len + NCOM_HEADER_LEN] = (uint8_t)(crc16 & 0xFF);
	txBuf[len + NCOM_HEADER_LEN + 1] = (uint8_t)((crc16 >> 8) & 0xFF);

	uint8_t status = CDC_Transmit_FS(txBuf, len + NCOM_HEADER_LEN + NCOM_FOOTER_LEN);

	if(status == USBD_OK) tx_seq++;

	return status;
}
#endif



/* #############################################
 * #############################################
 * ############################################# */
 
