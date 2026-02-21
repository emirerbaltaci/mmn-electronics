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



/* #############################################
 * 					 Variables
 * ############################################# */
extern CRC_HandleTypeDef hcrc;	// For Hardware CRC Calculation



/* #############################################
 * 			   Function Definitions
 * ############################################# */
 
/*
 *	NCOM_TX_SendPacket
 *	Transmits the message via USB OTG FS
 *
 *	Parameters: Message ID, Message Payload, Payload Size in Bytes
 *	Returns: USB Transmit Status
 */
uint8_t NCOM_TX_SendPacket(uint8_t msgId, const void* payload, uint8_t len){
	
	if(len > 128) return USBD_FAIL;
	
	static uint8_t tx_seq = 0;
	static uint8_t txBuf[135];
	
	txBuf[0] = NCOM_SYNC_BYTE_1;
	txBuf[1] = NCOM_SYNC_BYTE_2;
	txBuf[2] = tx_seq;
	txBuf[3] = msgId;
	txBuf[4] = len;
	
	if(len > 0 && payload != NULL) memcpy(&txBuf[5], payload, len);
	
	uint32_t crc = HAL_CRC_Calculate(&hcrc, (uint32_t*)&txBuf[2], len + 3);
	uint16_t crc16 = (uint16_t)crc;
	txBuf[len + 5] = (uint8_t)(crc16 & 0xFF);
	txBuf[len + 6] = (uint8_t)((crc16 >> 8) & 0xFF);
	
	uint8_t status = CDC_Transmit_FS(txBuf, len + 7);
	
	if(status == USBD_OK){
		tx_seq++;
	}
	
	return status;
}



/* #############################################
 * #############################################
 * ############################################# */
 