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
 * 		     ncom_rx.h
 *   NCOM MCU-Side Reception Module Header File
 * ############################################# */
#ifndef INC_NCOM_RX_H_
#define INC_NCOM_RX_H_



/* #############################################
 * 		     Includes
 * ############################################# */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ncom_protocol.h"



/* #############################################
 * 		    Enumerations
 * ############################################# */
typedef enum{	// Parser States
	NCOM_RX_STATE_SYNC,
	NCOM_RX_STATE_ID,
	NCOM_RX_STATE_LEN,
	NCOM_RX_STATE_PAYLOAD,
	NCOM_RX_STATE_CRC
} NCOM_RX_State_t;



/* #############################################
 * 		Structure Definitions
 * ############################################# */
typedef struct{	// Parser Structure
	NCOM_RX_State_t state;
	uint8_t msgId;
	uint8_t payloadLen;
	uint8_t payloadBuf[256];
	uint8_t payloadIndex;
	uint8_t crcBuf[2];
	uint8_t crcIndex;
} NCOM_RX_Parser_t;



/* #############################################
 * 		Function Prototypes
 * ############################################# */
void NCOM_RX_Init(NCOM_RX_Parser_t* parser);
bool NCOM_RX_ParseByte(NCOM_RX_Parser_t* parser, uint8_t byte);



/* #############################################
 * #############################################
 * ############################################# */
#endif /* INC_NCOM_RX_H_ */
