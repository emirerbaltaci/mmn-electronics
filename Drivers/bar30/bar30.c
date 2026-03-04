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

#include "bar30.h"
#include <stddef.h>

#include "FreeRTOS.h"
#include "task.h"

// Delay times based on OSR (in ms)
static uint32_t BAR30_GetDelayMs(uint8_t osr) {
  switch (osr) {
  case BAR30_OSR_256:
    return 1;
  case BAR30_OSR_512:
    return 2;
  case BAR30_OSR_1024:
    return 3;
  case BAR30_OSR_2048:
    return 5;
  case BAR30_OSR_4096:
    return 10;
  case BAR30_OSR_8192:
    return 20;
  default:
    return 20;
  }
}

BAR30_Status_t BAR30_Init(BAR30_Handler_t *bar) {
  if (bar == NULL || bar->pI2Cx == NULL)
    return BAR30_ERROR;

  bar->state = 0;
  bar->waitStartTime = 0;
  bar->D1 = 0;
  bar->D2 = 0;

  // Reset
  uint8_t cmd = BAR30_CMD_RESET;
  if (HAL_I2C_Master_Transmit(bar->pI2Cx, BAR30_I2C_ADDR, &cmd, 1, 100) !=
      HAL_OK) {
    return BAR30_ERROR;
  }
  HAL_Delay(10); // Wait for reset to complete

  // Read Calibration Coefficients (PROM)
  // PROM uses address 0xA0 to 0xAE (0-7, 7 is the CRC word usually but we read
  // 0-6 or 0-7)
  for (uint8_t i = 0; i < 7; i++) {
    cmd = BAR30_CMD_PROM_RD + (i * 2);
    if (HAL_I2C_Master_Transmit(bar->pI2Cx, BAR30_I2C_ADDR, &cmd, 1, 100) !=
        HAL_OK)
      return BAR30_ERROR;

    uint8_t buf[2];
    if (HAL_I2C_Master_Receive(bar->pI2Cx, BAR30_I2C_ADDR, buf, 2, 100) !=
        HAL_OK)
      return BAR30_ERROR;

    bar->calCoeff[i] = (uint16_t)((buf[0] << 8) | buf[1]);
  }

  return BAR30_OK;
}

// Non-blocking update function suitable for RTOS.
BAR30_Status_t BAR30_Update(BAR30_Handler_t *bar) {
  if (bar == NULL || bar->pI2Cx == NULL)
    return BAR30_ERROR;

  uint8_t cmd;
  uint8_t buf[3];
  uint32_t waitTime = BAR30_GetDelayMs(bar->config.osr);
  uint32_t now = HAL_GetTick();

  switch (bar->state) {
  case 0:
    cmd = BAR30_CMD_CONV_D1_BASE + bar->config.osr;
    if (HAL_I2C_Master_Transmit(bar->pI2Cx, BAR30_I2C_ADDR, &cmd, 1, 100) !=
        HAL_OK) {
      bar->state = 0;
      return BAR30_ERROR;
    }
    bar->waitStartTime = now;
    bar->state = 1;
    return BAR30_BUSY;

  case 1:
    if ((now - bar->waitStartTime) >= waitTime) {
      cmd = BAR30_CMD_ADC_READ;
      if (HAL_I2C_Master_Transmit(bar->pI2Cx, BAR30_I2C_ADDR, &cmd, 1, 100) !=
          HAL_OK) {
        bar->state = 0;
        return BAR30_ERROR;
      }
      if (HAL_I2C_Master_Receive(bar->pI2Cx, BAR30_I2C_ADDR, buf, 3, 100) !=
          HAL_OK) {
        bar->state = 0;
        return BAR30_ERROR;
      }

      bar->D1 = (buf[0] << 16) | (buf[1] << 8) | buf[2];

      cmd = BAR30_CMD_CONV_D2_BASE + bar->config.osr;
      if (HAL_I2C_Master_Transmit(bar->pI2Cx, BAR30_I2C_ADDR, &cmd, 1, 100) !=
          HAL_OK) {
        bar->state = 0;
        return BAR30_ERROR;
      }

      bar->waitStartTime = HAL_GetTick();
      bar->state = 2;
    }
    return BAR30_BUSY;

  case 2:
    if ((now - bar->waitStartTime) >= waitTime) {
      cmd = BAR30_CMD_ADC_READ;
      if (HAL_I2C_Master_Transmit(bar->pI2Cx, BAR30_I2C_ADDR, &cmd, 1, 100) !=
          HAL_OK) {
        bar->state = 0;
        return BAR30_ERROR;
      }
      if (HAL_I2C_Master_Receive(bar->pI2Cx, BAR30_I2C_ADDR, buf, 3, 100) !=
          HAL_OK) {
        bar->state = 0;
        return BAR30_ERROR;
      }

      bar->D2 = (buf[0] << 16) | (buf[1] << 8) | buf[2];

      // Compute math using D1, D2
      uint32_t coeff_Tref = bar->calCoeff[5];
      int32_t dT = (int32_t)bar->D2 - (int32_t)(coeff_Tref * 256);
      int64_t SENS = ((int64_t)bar->calCoeff[1] * 32768) +
                     (((int64_t)bar->calCoeff[3] * dT) / 256);
      int64_t OFF = ((int64_t)bar->calCoeff[2] * 65536) +
                    (((int64_t)bar->calCoeff[4] * dT) / 128);
      int32_t TEMP =
          2000 + (int32_t)(((int64_t)dT * bar->calCoeff[6]) / 8388608);

      // Second order temp compensation
      int64_t Ti = 0;
      int64_t OFFi = 0;
      int64_t SENSi = 0;

      if (TEMP < 2000) {
        Ti = (3 * (int64_t)dT * dT) / 8589934592LL;
        OFFi = 3 * ((TEMP - 2000) * (int64_t)(TEMP - 2000)) / 2;
        SENSi = 5 * ((TEMP - 2000) * (int64_t)(TEMP - 2000)) / 8;
        if (TEMP < -1500) {
          OFFi += 7 * ((TEMP + 1500) * (int64_t)(TEMP + 1500));
          SENSi += 4 * ((TEMP + 1500) * (int64_t)(TEMP + 1500));
        }
      } else {
        Ti = (2 * (int64_t)dT * dT) / 137438953472LL;
        OFFi = 1 * ((TEMP - 2000) * (int64_t)(TEMP - 2000)) / 16;
        SENSi = 0;
      }

      int64_t OFF2 = OFF - OFFi;
      int64_t SENS2 = SENS - SENSi;
      int32_t TEMP2 = (int32_t)(TEMP - (int32_t)Ti);

      int32_t P = ((((int64_t)bar->D1 * SENS2) / 2097152) - OFF2) / 8192;

      bar->data.pressure = P / 10.0f;         // mbar
      bar->data.temperature = TEMP2 / 100.0f; // deg C

      // Depth calculation: P = P_surface + rho * g * z -> z = (P_abs -
      // P_surface) * 100 / (rho * g) Standard atmospheric pressure: 1013.25
      float safe_density = bar->config.density;
      if (safe_density < 1.0f)
        safe_density = 1000.0f; // Guard against uninitialized div zero

      bar->depth =
          (bar->data.pressure - 1013.25f) * 100.0f / (safe_density * 9.80665f);

      bar->state = 0;
      return BAR30_OK;
    }
    return BAR30_BUSY;
  }
  return BAR30_ERROR;
}

// Blocking GetData function
BAR30_Status_t BAR30_GetData(BAR30_Handler_t *bar) {
  BAR30_Status_t status;
  do {
    status = BAR30_Update(bar);
    if (status == BAR30_BUSY)
      vTaskDelay(1);
  } while (status == BAR30_BUSY);
  return status;
}
