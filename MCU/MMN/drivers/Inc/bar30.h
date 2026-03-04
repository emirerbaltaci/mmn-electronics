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

#ifndef DRIVERS_INC_BAR30_H_
#define DRIVERS_INC_BAR30_H_

#include "stm32g4xx_hal.h"

typedef enum {
  BAR30_OK = 0,
  BAR30_ERROR,
  BAR30_BUSY,
  BAR30_TIMEOUT,
  BAR30_INVALID_CONFIG
} BAR30_Status_t;

typedef enum {
  BAR30_STATE_IDLE = 0,
  BAR30_STATE_WAIT_D1,
  BAR30_STATE_WAIT_D2
} BAR30_State_t;

#define BAR30_I2C_ADDR (0x76 << 1)

#define BAR30_CMD_RESET 0x1E
#define BAR30_CMD_ADC_READ 0x00
#define BAR30_CMD_PROM_RD 0xA0

// OSR offsets for commands
#define BAR30_OSR_256 0x00
#define BAR30_OSR_512 0x02
#define BAR30_OSR_1024 0x04
#define BAR30_OSR_2048 0x06
#define BAR30_OSR_4096 0x08
#define BAR30_OSR_8192 0x0A

// Conversion start commands:
#define BAR30_CMD_CONV_D1_BASE 0x40
#define BAR30_CMD_CONV_D2_BASE 0x50

typedef struct {
  uint8_t osr;
  float density;
} BAR30_Config_t;

typedef struct {
  float pressure;    // mbar
  float temperature; // deg C
} BAR30_Data_t;

typedef struct {
  I2C_HandleTypeDef *pI2Cx;
  BAR30_Config_t config;
  BAR30_Data_t data;
  uint16_t calCoeff[7];
  float depth;
  BAR30_State_t state;
  uint32_t waitStartTime;
  uint32_t D1;
  uint32_t D2;
} BAR30_Handler_t;

BAR30_Status_t BAR30_Init(BAR30_Handler_t *bar);
BAR30_Status_t BAR30_Update(BAR30_Handler_t *bar);

// Note: Uses vTaskDelay — must be called from within an active FreeRTOS task.
BAR30_Status_t BAR30_GetData(BAR30_Handler_t *bar);

#endif /* DRIVERS_INC_BAR30_H_ */
