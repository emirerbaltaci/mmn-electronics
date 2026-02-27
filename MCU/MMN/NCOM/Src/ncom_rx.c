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
 * 					 ncom_rx.c
 *   NCOM MCU-Side Reception Module Source File
 * ############################################# */
 
 
 
/* #############################################
 * 					Includes
 * ############################################# */
#include "ncom_rx.h"
#include "main.h"



/* #############################################
 * 					Variables
 * ############################################# */
extern CRC_HandleTypeDef hcrc;	// For Hardware CRC Calculation



/* #############################################
 * 			   Function Definitions
 * ############################################# */

/*
 *	NCOM_RX_Init
 *	Resets parser state to wait for sync byte
 *
 *	Parameters: NCOM RX Parser Structure
 *	Returns: -
 */
void NCOM_RX_Init(NCOM_RX_t* rx){
	rx->parser.state = NCOM_RX_STATE_SYNC_1;
	rx->ringBuffer.head = 0;	
	rx->ringBuffer.tail = 0;
}

/*
 *	NCOM_RX_ParseByte
 *	Main parser function, processes the received byte according to the current parser state
 *
 *	Parameters: NCOM RX Parser Structure, Received Byte
 *	Returns: true (valid data) or false (invalid data)
 */
bool NCOM_RX_ParseByte(NCOM_RX_t* rx, uint8_t byte){
	
	switch (rx->parser.state) {
		
		case NCOM_RX_STATE_SYNC_1:
			if (byte == NCOM_SYNC_BYTE_1) rx->parser.state = NCOM_RX_STATE_SYNC_2;
			break;
			
		case NCOM_RX_STATE_SYNC_2:
			if (byte == NCOM_SYNC_BYTE_2) rx->parser.state = NCOM_RX_STATE_SEQ;
			else if (byte == NCOM_SYNC_BYTE_1) break;
			else rx->parser.state = NCOM_RX_STATE_SYNC_1;
			break;
			
		case NCOM_RX_STATE_SEQ:
			rx->parser.seq = byte;
			rx->parser.state = NCOM_RX_STATE_ID;
			break;
		
		case NCOM_RX_STATE_ID:
			rx->parser.msgId = byte;
			rx->parser.state = NCOM_RX_STATE_LEN;
			break;
		
		case NCOM_RX_STATE_LEN:
			if(byte > NCOM_MAX_PAYLOAD_LEN){
				rx->parser.state = NCOM_RX_STATE_SYNC_1;
				break;
			}
			rx->parser.payloadLen = byte;
			rx->parser.payloadIndex = 0;
			if(rx->parser.payloadLen == 0){
				rx->parser.state = NCOM_RX_STATE_CRC;
				rx->parser.crcIndex = 0;
			}
			else rx->parser.state = NCOM_RX_STATE_PAYLOAD;
			break;
		
		case NCOM_RX_STATE_PAYLOAD:
			rx->parser.payloadBuf[rx->parser.payloadIndex++] = byte;
			if(rx->parser.payloadIndex >= rx->parser.payloadLen){
				rx->parser.state = NCOM_RX_STATE_CRC;
				rx->parser.crcIndex = 0;
			}
			break;
		
		case NCOM_RX_STATE_CRC:
			rx->parser.crcBuf[rx->parser.crcIndex++] = byte;
			if(rx->parser.crcIndex >= 2){
				rx->parser.state = NCOM_RX_STATE_SYNC_1;
				uint16_t crcRx = (uint16_t)rx->parser.crcBuf[0] | ((uint16_t)rx->parser.crcBuf[1] << 8);
				__HAL_CRC_DR_RESET(&hcrc);
				uint8_t header[3] = {rx->parser.seq, rx->parser.msgId, rx->parser.payloadLen};
				for(int i = 0; i < 3; i++) *(uint8_t*)(&hcrc.Instance->DR) = header[i];
				for(int i = 0; i < rx->parser.payloadLen; i++) *(uint8_t*)(&hcrc.Instance->DR) = rx->parser.payloadBuf[i];
				if((uint16_t)(hcrc.Instance->DR & 0xFFFF) == crcRx) return true;
			}
			break;
	}
	return false;
}

/*
 *	NCOM_RX_RingBuffer_Write
 *	Writes data to the ring buffer.
 *
 *	Parameters: NCOM RX Structure, Pointer to data, Length of data
 *	Returns: -
 */
void NCOM_RX_RingBuffer_Write(NCOM_RX_t* rx, uint8_t* data, uint16_t len){
	for(uint16_t i = 0; i < len; i++){
		uint16_t next_head = (rx->ringBuffer.head + 1) & (NCOM_RINGBUFFER_SIZE - 1);
		if(next_head == rx->ringBuffer.tail) return;
		rx->ringBuffer.buf[rx->ringBuffer.head] = data[i];
		rx->ringBuffer.head = next_head;
	}
}

/*
 *	NCOM_RX_RingBuffer_Read
 *	Reads a byte from the ring buffer
 *
 *	Parameters: NCOM RX Structure, Byte buffer
 *	Returns: false if the ring buffer is empty, true otherwise.
 */
bool NCOM_RX_RingBuffer_Read(NCOM_RX_t* rx, uint8_t* byte){
	if(rx->ringBuffer.head == rx->ringBuffer.tail) return false;
	*byte = rx->ringBuffer.buf[rx->ringBuffer.tail];
	rx->ringBuffer.tail = (rx->ringBuffer.tail + 1) & (NCOM_RINGBUFFER_SIZE - 1);
	return true;
}

/*
 *	NCOM_RX_RingBuffer_GetSize
 *	Gets the size of the ring buffer
 *
 *	Parameters: NCOM RX Parser Structure
 *	Returns: Size of the ring buffer
 */
uint16_t NCOM_RX_RingBuffer_GetSize(NCOM_RX_t* rx){
	return (rx->ringBuffer.head - rx->ringBuffer.tail) & (NCOM_RINGBUFFER_SIZE - 1);
}



/* #############################################
 * #############################################
 * ############################################# */
