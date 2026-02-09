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
 * 		     ncom_rx.c
 *   NCOM MCU-Side Reception Module Source File
 * ############################################# */
 
 
 
/* #############################################
 * 		     Includes
 * ############################################# */
#include "ncom_rx.h"
#include "main.h"



/* #############################################
 * 		     Variables
 * ############################################# */
extern CRC_HandleTypeDef hcrc;	// For Hardware CRC Calculation



/* #############################################
 * 		Function Definitions
 * ############################################# */

/*
 *	NCOM_RX_Init
 *	Resets parser state to wait for sync byte
 *
 *	Parameters: NCOM RX Parser Structure
 *	Returns: -
 */
void NCOM_RX_Init(NCOM_RX_Parser_t* parser){
	parser->state = NCOM_RX_STATE_SYNC;
}

/*
 *	NCOM_RX_ParseByte
 *	Main parser function, processes the received byte according to the current parser state
 *
 *	Parameters: NCOM RX Parser Structure, Received Byte
 *	Returns: true (valid data) or false (invalid data)
 */
bool NCOM_RX_ParseByte(NCOM_RX_Parser_t* parser, uint8_t byte){
	
	switch (parser->state) {
		
		case NCOM_RX_STATE_SYNC:
			if (byte == NCOM_SYNC_BYTE) parser->state = NCOM_RX_STATE_ID;
			break;
		
		case NCOM_RX_STATE_ID:
			parser->msgId = byte;
			parser->state = NCOM_RX_STATE_LEN;
			break;
		
		case NCOM_RX_STATE_LEN:
			parser->payloadLen = byte;
			parser->payloadIndex = 0;
			if(parser->payloadLen == 0){
				parser->state = NCOM_RX_STATE_CRC;
				parser->crcIndex = 0;
			}
			else parser->state = NCOM_RX_STATE_PAYLOAD;
			break;
		
		case NCOM_RX_STATE_PAYLOAD:
			parser->payloadBuf[parser->payloadIndex++] = byte;
			if(parser->payloadIndex >= parser->payloadLen){
				parser->state = NCOM_RX_STATE_CRC;
				parser->crcIndex = 0;
			}
			break;
		
		case NCOM_RX_STATE_CRC:
			parser->crcBuf[parser->crcIndex++] = byte;
			if(parser->crcIndex >= 2){
				parser->state = NCOM_RX_STATE_SYNC;
				uint16_t crcRx = (uint16_t)parser->crcBuf[0] | ((uint16_t)parser->crcBuf[1] << 8);
				uint8_t header[2] = {parser->msgId, parser->payloadLen};
				uint32_t crcCalc = HAL_CRC_Calculate(&hcrc, (uint32_t*)header, 2);
				if(parser->payloadLen > 0) crcCalc = HAL_CRC_Accumulate(&hcrc, (uint32_t*)parser->payloadBuf, parser->payloadLen);
				if((uint16_t)crcCalc == crcRx) return true;
			}
			break;
	}
	return false;
}



/* #############################################
 * #############################################
 * ############################################# */
 