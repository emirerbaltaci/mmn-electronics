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

#include "imu.h"
#include "auv_setup.h"
#include "auvconfig.h"
#include <math.h>
#include <string.h>


static inline void IMU_SPI_Enable(IMU_Handler_t *imu) {
  HAL_GPIO_WritePin(imu->pGPIOx, imu->GPIO_PIN_x, GPIO_PIN_RESET);
}

static inline void IMU_SPI_Disable(IMU_Handler_t *imu) {
  HAL_GPIO_WritePin(imu->pGPIOx, imu->GPIO_PIN_x, GPIO_PIN_SET);
}

static inline void IMU_SPI_NewCommand(IMU_Handler_t *imu) {
  if (!(imu->pGPIOx->ODR & imu->GPIO_PIN_x)) {
    IMU_SPI_Disable(imu);
    for (volatile int i = 0; i < 10; i++)
      __NOP();
  }
  IMU_SPI_Enable(imu);
}

IMU_Status_t IMU_SPI_ReadReg(IMU_Handler_t *imu, uint8_t REG,
                             uint8_t *rxBuffer) {
  uint8_t addr = REG | 0x80;
  IMU_SPI_NewCommand(imu);
  if (HAL_SPI_Transmit(imu->pSPIx, &addr, 1, 10) != HAL_OK) {
    IMU_SPI_Disable(imu);
    return IMU_ERROR;
  }
  if (HAL_SPI_Receive(imu->pSPIx, rxBuffer, 1, 10) != HAL_OK) {
    IMU_SPI_Disable(imu);
    return IMU_ERROR;
  }
  IMU_SPI_Disable(imu);
  return IMU_OK;
}

IMU_Status_t IMU_SPI_WriteReg(IMU_Handler_t *imu, uint8_t REG, uint8_t txData) {
  uint8_t msg[2] = {REG & 0x7F, txData};
  IMU_SPI_NewCommand(imu);
  if (HAL_SPI_Transmit(imu->pSPIx, msg, 2, 10) != HAL_OK) {
    IMU_SPI_Disable(imu);
    return IMU_ERROR;
  }
  IMU_SPI_Disable(imu);
  return IMU_OK;
}

IMU_Status_t IMU_SPI_ReadBurst(IMU_Handler_t *imu, uint8_t REG,
                               uint8_t *rxBuffer, uint8_t size) {
  uint8_t addr = REG | 0x80;
  IMU_SPI_NewCommand(imu);
  if (HAL_SPI_Transmit(imu->pSPIx, &addr, 1, 10) != HAL_OK) {
    IMU_SPI_Disable(imu);
    return IMU_ERROR;
  }
  if (HAL_SPI_Receive(imu->pSPIx, rxBuffer, size, 10) != HAL_OK) {
    IMU_SPI_Disable(imu);
    return IMU_ERROR;
  }
  IMU_SPI_Disable(imu);
  return IMU_OK;
}

IMU_Status_t IMU_SPI_WriteBurst(IMU_Handler_t *imu, uint8_t REG,
                                uint8_t *txBuffer, uint8_t size) {
  uint8_t addr = REG & 0x7F;
  IMU_SPI_NewCommand(imu);
  if (HAL_SPI_Transmit(imu->pSPIx, &addr, 1, 10) != HAL_OK) {
    IMU_SPI_Disable(imu);
    return IMU_ERROR;
  }
  if (HAL_SPI_Transmit(imu->pSPIx, txBuffer, size, 10) != HAL_OK) {
    IMU_SPI_Disable(imu);
    return IMU_ERROR;
  }
  IMU_SPI_Disable(imu);
  return IMU_OK;
}

static inline void IMU_MultiplierHelper(IMU_Handler_t *imu) {
  uint8_t accel_fs = imu->config.accel_config0 & 0xE0;
  uint8_t gyro_fs = imu->config.gyro_config0 & 0xE0;

  switch (accel_fs) {
  case IMU_ACCEL_UI_FS_SEL_16G:
    imu->accelMult = 1.0f / 2048.0f;
    break;
  case IMU_ACCEL_UI_FS_SEL_8G:
    imu->accelMult = 1.0f / 4096.0f;
    break;
  case IMU_ACCEL_UI_FS_SEL_4G:
    imu->accelMult = 1.0f / 8192.0f;
    break;
  case IMU_ACCEL_UI_FS_SEL_2G:
    imu->accelMult = 1.0f / 16384.0f;
    break;
  default:
    imu->accelMult = 1.0f / 2048.0f;
    break;
  }

  switch (gyro_fs) {
  case IMU_GYRO_UI_FS_SEL_2000DPS:
    imu->gyroMult = 1.0f / 16.4f;
    break;
  case IMU_GYRO_UI_FS_SEL_1000DPS:
    imu->gyroMult = 1.0f / 32.8f;
    break;
  case IMU_GYRO_UI_FS_SEL_500DPS:
    imu->gyroMult = 1.0f / 65.5f;
    break;
  case IMU_GYRO_UI_FS_SEL_250DPS:
    imu->gyroMult = 1.0f / 131.0f;
    break;
  case IMU_GYRO_UI_FS_SEL_125DPS:
    imu->gyroMult = 1.0f / 262.0f;
    break;
  case IMU_GYRO_UI_FS_SEL_62_5DPS:
    imu->gyroMult = 1.0f / 524.3f;
    break;
  case IMU_GYRO_UI_FS_SEL_31_25DPS:
    imu->gyroMult = 1.0f / 1048.6f;
    break;
  case IMU_GYRO_UI_FS_SEL_15_625DPS:
    imu->gyroMult = 1.0f / 2097.2f;
    break;
  default:
    imu->gyroMult = 1.0f / 16.4f;
    break;
  }
}

static inline void IMU_SetRegBank(IMU_Handler_t *imu, uint8_t BANK_SEL_x) {
  IMU_SPI_WriteReg(imu, IMU_REG_REG_BANK_SEL, BANK_SEL_x);
}

IMU_Status_t IMU_SPI_Init(IMU_Handler_t *imu) {
  uint8_t temp = 0;
  // Register Bank 0
  IMU_SetRegBank(imu, IMU_BANK_SEL_0);

  // Soft Reset, DEVICE_CONFIG and WHO_AM_I
  if (IMU_SPI_WriteReg(imu, IMU_REG_DEVICE_CONFIG, IMU_SOFT_RESET_CONFIG_1) !=
      IMU_OK)
    return IMU_ERROR;
  HAL_Delay(1);
  if (IMU_SPI_ReadReg(imu, IMU_REG_WHO_AM_I, &temp) != IMU_OK)
    return IMU_ERROR;
  if (temp != IMU_WHOAMI)
    return IMU_ERROR;

  // Load config headers from auvConfig
  IMU_Config_LoadConfigHeader(&imu->config);

  // DEVICE_CONFIG (RMW: Preserve 7:5, 3:1. Clear 4,0 for Config)
  if (IMU_SPI_ReadReg(imu, IMU_REG_DEVICE_CONFIG, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xEE;
  temp |= (imu->config.device_config & 0x11);
  if (IMU_SPI_WriteReg(imu, IMU_REG_DEVICE_CONFIG, temp) != IMU_OK)
    return IMU_ERROR;

  // PWR_MGMT0 (RMW: Preserve 7:6)
  if (IMU_SPI_ReadReg(imu, IMU_REG_PWR_MGMT0, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xC0;
  temp |= (imu->config.pwr_mgmt0 & 0x3F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_PWR_MGMT0, temp) != IMU_OK)
    return IMU_ERROR;

  // DRIVE_CONFIG (RMW: Preserve 7:6)
  if (IMU_SPI_ReadReg(imu, IMU_REG_DRIVE_CONFIG, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xC0;
  temp |= (imu->config.drive_config & 0x3F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_DRIVE_CONFIG, temp) != IMU_OK)
    return IMU_ERROR;

  // INT_CONFIG (RMW: Preserve 7:6)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_CONFIG, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xC0;
  temp |= (imu->config.int_config & 0x3F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_CONFIG, temp) != IMU_OK)
    return IMU_ERROR;

  // FIFO_CONFIG (RMW: Preserve 5:0. Config 7:6)
  if (IMU_SPI_ReadReg(imu, IMU_REG_FIFO_CONFIG, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x3F;
  temp |= (imu->config.fifo_config & 0xC0);
  if (IMU_SPI_WriteReg(imu, IMU_REG_FIFO_CONFIG, temp) != IMU_OK)
    return IMU_ERROR;

  // INTF_CONFIG0 (RMW: Preserve 3:2. Config 7:4, 1:0)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INTF_CONFIG0, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x0C;
  temp |= (imu->config.intf_config0 & 0xF3);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INTF_CONFIG0, temp) != IMU_OK)
    return IMU_ERROR;

  // INTF_CONFIG1 (RMW: Preserve 7:4. Config 3:0)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INTF_CONFIG1, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xF0;
  temp |= (imu->config.intf_config1 & 0x0F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INTF_CONFIG1, temp) != IMU_OK)
    return IMU_ERROR;

  // GYRO_CONFIG0 (RMW: Preserve 4. Config 7:5, 3:0)
  if (IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG0, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x10;
  temp |= (imu->config.gyro_config0 & 0xEF);
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG0, temp) != IMU_OK)
    return IMU_ERROR;

  // ACCEL_CONFIG0 (RMW: Preserve 4. Config 7:5, 3:0)
  if (IMU_SPI_ReadReg(imu, IMU_REG_ACCEL_CONFIG0, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x10;
  temp |= (imu->config.accel_config0 & 0xEF);
  if (IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_CONFIG0, temp) != IMU_OK)
    return IMU_ERROR;

  // GYRO_CONFIG1 (RMW: Preserve 4,1,0. Config 7:5, 3:2)
  if (IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG1, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x13;
  temp |= (imu->config.gyro_config1 & 0xEC);
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG1, temp) != IMU_OK)
    return IMU_ERROR;

  // ACCEL_CONFIG1 (RMW: Preserve 7:5, 2:0. Config 4:3. Original was &= 0xE7)
  // Original code preserved 7:5, 2:0.
  if (IMU_SPI_ReadReg(imu, IMU_REG_ACCEL_CONFIG1, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xE7;
  temp |= (imu->config.accel_config1 & 0x18);
  if (IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_CONFIG1, temp) != IMU_OK)
    return IMU_ERROR;

  // GYRO_ACCEL_CONFIG0 - Direct Write
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_ACCEL_CONFIG0,
                       imu->config.gyro_accel_config0) != IMU_OK)
    return IMU_ERROR;

  // TMST_CONFIG (RMW: Preserve 7:5)
  if (IMU_SPI_ReadReg(imu, IMU_REG_TMST_CONFIG, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xE0;
  temp |= (imu->config.tmst_config & 0x1F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_TMST_CONFIG, temp) != IMU_OK)
    return IMU_ERROR;

  // FIFO_CONFIG1 (RMW: Preserve 7)
  if (IMU_SPI_ReadReg(imu, IMU_REG_FIFO_CONFIG1, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x80;
  temp |= (imu->config.fifo_config1 & 0x7F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_FIFO_CONFIG1, temp) != IMU_OK)
    return IMU_ERROR;

  // FIFO_CONFIG2, 3
  if (IMU_SPI_WriteReg(imu, IMU_REG_FIFO_CONFIG2, imu->config.fifo_config2) !=
      IMU_OK)
    return IMU_ERROR;

  // FIFO_CONFIG3 (RMW: Preserve 7:4)
  if (IMU_SPI_ReadReg(imu, IMU_REG_FIFO_CONFIG3, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xF0;
  temp |= (imu->config.fifo_config3 & 0x0F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_FIFO_CONFIG3, temp) != IMU_OK)
    return IMU_ERROR;

  // FSYNC_CONFIG (RMW: Preserve 7, 3:2)
  if (IMU_SPI_ReadReg(imu, IMU_REG_FSYNC_CONFIG, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x8C;
  temp |= (imu->config.fsync_config & 0x73);
  if (IMU_SPI_WriteReg(imu, IMU_REG_FSYNC_CONFIG, temp) != IMU_OK)
    return IMU_ERROR;

  // SIGNAL_PATH_RESET - Flush FIFO (RMW: Preserve 7, 4, 0)
  if (IMU_SPI_ReadReg(imu, IMU_REG_SIGNAL_PATH_RESET, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x91;
  temp |= IMU_FIFO_FLUSH;
  if (IMU_SPI_WriteReg(imu, IMU_REG_SIGNAL_PATH_RESET, temp) != IMU_OK)
    return IMU_ERROR;

  // INT_CONFIG0 (RMW: Preserve 7:6)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_CONFIG0, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xC0;
  temp |= (imu->config.int_config0 & 0x3F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_CONFIG0, temp) != IMU_OK)
    return IMU_ERROR;

  // INT_CONFIG1 (RMW: Preserve 7, 3:0. Original &= 0x8F. Config 6:4)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_CONFIG1, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x8F;
  temp |= (imu->config.int_config1 & 0x70);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_CONFIG1, temp) != IMU_OK)
    return IMU_ERROR;

  // INT_SOURCE0 (RMW: Preserve 7)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_SOURCE0, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x80;
  temp |= (imu->config.int_source0 & 0x7F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_SOURCE0, temp) != IMU_OK)
    return IMU_ERROR;

  // INT_SOURCE1 (RMW: Preserve 7)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_SOURCE1, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x80;
  temp |= (imu->config.int_source1 & 0x7F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_SOURCE1, temp) != IMU_OK)
    return IMU_ERROR;

  // INT_SOURCE3 (RMW: Preserve 7)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_SOURCE3, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x80;
  temp |= (imu->config.int_source3 & 0x7F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_SOURCE3, temp) != IMU_OK)
    return IMU_ERROR;

  // INT_SOURCE4 (RMW: Preserve 7)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_SOURCE4, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x80;
  temp |= (imu->config.int_source4 & 0x7F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_SOURCE4, temp) != IMU_OK)
    return IMU_ERROR;

  // SELF_TEST_CONFIG (RMW: Preserve 7)
  if (IMU_SPI_ReadReg(imu, IMU_REG_SELF_TEST_CONFIG, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x80;
  temp |= (imu->config.self_test_config & 0x7F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_SELF_TEST_CONFIG, temp) != IMU_OK)
    return IMU_ERROR;

  IMU_SetRegBank(imu, IMU_BANK_SEL_1);

  // SENSOR_CONFIG0 (RMW: Preserve 7:6)
  if (IMU_SPI_ReadReg(imu, IMU_REG_SENSOR_CONFIG0, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xC0;
  temp |= (imu->config.sensor_config0 & 0x3F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_SENSOR_CONFIG0, temp) != IMU_OK)
    return IMU_ERROR;

  // CLKDIV (RMW: Preserve 7)
  IMU_SetRegBank(imu, IMU_BANK_SEL_3);
  if (IMU_SPI_ReadReg(imu, IMU_REG_CLKDIV, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x80;
  temp |= (imu->config.clkdiv & 0x7F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_CLKDIV, temp) != IMU_OK)
    return IMU_ERROR;
  IMU_SetRegBank(imu, IMU_BANK_SEL_1);

  // GYRO_CONFIG_STATIC2 (RMW: Preserve 7:2)
  if (IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG_STATIC2, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xFC;
  temp |= (imu->config.gyro_config_static2 & 0x03);
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC2, temp) != IMU_OK)
    return IMU_ERROR;

  // GYRO_CONFIG_STATIC3 (RMW: Preserve 7:6)
  if (IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG_STATIC3, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xC0;
  temp |= (imu->config.gyro_config_static3 & 0x3F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC3, temp) != IMU_OK)
    return IMU_ERROR;

  // Direct Writes for calculated filters (Static 4-8)
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC4,
                       imu->config.gyro_config_static4) != IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC5,
                       imu->config.gyro_config_static5) != IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC6,
                       imu->config.gyro_config_static6) != IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC7,
                       imu->config.gyro_config_static7) != IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC8,
                       imu->config.gyro_config_static8) != IMU_OK)
    return IMU_ERROR;

  // GYRO_CONFIG_STATIC9 (RMW: Preserve 7:6)
  if (IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG_STATIC9, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xC0;
  temp |= (imu->config.gyro_config_static9 & 0x3F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC9, temp) != IMU_OK)
    return IMU_ERROR;

  // GYRO_CONFIG_STATIC10 (RMW: Preserve 7, 3:0. Original &= 0x8F. Config 6:4)
  if (IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG_STATIC10, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x8F;
  temp |= (imu->config.gyro_config_static10 & 0x70);
  if (IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC10, temp) != IMU_OK)
    return IMU_ERROR;

  // INTF_CONFIG4 (RMW: Preserve 7,5,4,3,2,0. Original &= 0xBD -> Clear 6,1)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INTF_CONFIG4, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xBD;
  temp |= (imu->config.intf_config4 & 0x42);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INTF_CONFIG4, temp) != IMU_OK)
    return IMU_ERROR;

  // INTF_CONFIG5 (RMW: Preserve 7:3, 0. Original &= 0xF9. Config 2:1)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INTF_CONFIG5, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xF9;
  temp |= (imu->config.intf_config5 & 0x06);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INTF_CONFIG5, temp) != IMU_OK)
    return IMU_ERROR;

  IMU_SetRegBank(imu, IMU_BANK_SEL_2);
  // ACCEL_CONFIG_STATIC2 (RMW: Preserve 7. Original &= 0x80)
  if (IMU_SPI_ReadReg(imu, IMU_REG_ACCEL_CONFIG_STATIC2, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x80;
  temp |= (imu->config.accel_config_static2 & 0x7F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_CONFIG_STATIC2, temp) != IMU_OK)
    return IMU_ERROR;

  if (IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_CONFIG_STATIC3,
                       imu->config.accel_config_static3) != IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_CONFIG_STATIC4,
                       imu->config.accel_config_static4) != IMU_OK)
    return IMU_ERROR;

  IMU_SetRegBank(imu, IMU_BANK_SEL_4);

  // APEX_CONFIG (1-9)
  if (IMU_SPI_WriteReg(imu, IMU_REG_APEX_CONFIG1, imu->config.apex_config1) !=
      IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_APEX_CONFIG2, imu->config.apex_config2) !=
      IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_APEX_CONFIG3, imu->config.apex_config3) !=
      IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_APEX_CONFIG4, imu->config.apex_config4) !=
      IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_APEX_CONFIG5, imu->config.apex_config5) !=
      IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_APEX_CONFIG6, imu->config.apex_config6) !=
      IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_APEX_CONFIG7, imu->config.apex_config7) !=
      IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_APEX_CONFIG8, imu->config.apex_config8) !=
      IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_APEX_CONFIG9, imu->config.apex_config9) !=
      IMU_OK)
    return IMU_ERROR;

  // ACCEL_WOM (X, Y, Z)
  if (IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_WOM_X_THR,
                       imu->config.accel_wom_x_thr) != IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_WOM_Y_THR,
                       imu->config.accel_wom_y_thr) != IMU_OK)
    return IMU_ERROR;
  if (IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_WOM_Z_THR,
                       imu->config.accel_wom_z_thr) != IMU_OK)
    return IMU_ERROR;

  // INT_SOURCE (6-9)
  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_SOURCE6, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xC0; // Preserve 7:6
  temp |= (imu->config.int_source6 & 0x3F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_SOURCE6, temp) != IMU_OK)
    return IMU_ERROR;

  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_SOURCE7, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xC0; // Preserve 7:6
  temp |= (imu->config.int_source7 & 0x3F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_SOURCE7, temp) != IMU_OK)
    return IMU_ERROR;

  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_SOURCE8, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0xC0; // Preserve 7:6
  temp |= (imu->config.int_source8 & 0x3F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_SOURCE8, temp) != IMU_OK)
    return IMU_ERROR;

  if (IMU_SPI_ReadReg(imu, IMU_REG_INT_SOURCE9, &temp) != IMU_OK)
    return IMU_ERROR;
  temp &= 0x80; // Preserve 7
  temp |= (imu->config.int_source9 & 0x7F);
  if (IMU_SPI_WriteReg(imu, IMU_REG_INT_SOURCE9, temp) != IMU_OK)
    return IMU_ERROR;

  // OFFSET_USER (0-8)
  uint8_t offsets[9] = {imu->config.offset_user0, imu->config.offset_user1,
                        imu->config.offset_user2, imu->config.offset_user3,
                        imu->config.offset_user4, imu->config.offset_user5,
                        imu->config.offset_user6, imu->config.offset_user7,
                        imu->config.offset_user8};
  if (IMU_SPI_WriteBurst(imu, IMU_REG_OFFSET_USER0, offsets, 9) != IMU_OK)
    return IMU_ERROR;

  IMU_SetRegBank(imu, IMU_BANK_SEL_0);

  IMU_MultiplierHelper(imu);

  return IMU_OK;
}

static inline int16_t IMU_CombineLH_Signed(IMU_Handler_t *imu,
                                           uint8_t higherBits,
                                           uint8_t lowerBits) {
  if (imu->config.intf_config0 & IMU_SENSOR_DATA_ENDIAN_BIG)
    return (int16_t)((((uint16_t)higherBits) << 8) | lowerBits);
  else
    return (int16_t)((((uint16_t)lowerBits) << 8) | higherBits);
}

static inline uint16_t IMU_CombineLH_Unsigned(IMU_Handler_t *imu,
                                              uint8_t higherBits,
                                              uint8_t lowerBits) {
  if (imu->config.intf_config0 & IMU_SENSOR_DATA_ENDIAN_BIG)
    return (uint16_t)((((uint16_t)higherBits) << 8) | lowerBits);
  else
    return (uint16_t)((((uint16_t)lowerBits) << 8) | higherBits);
}

IMU_Status_t IMU_SPI_ReadAccel(IMU_Handler_t *imu, IMU_Data_t *data) {
  uint8_t buf[6];
  int16_t buf2[3];

  if (IMU_SPI_ReadBurst(imu, IMU_REG_ACCEL_DATA_X1, buf, 6) != IMU_OK)
    return IMU_ERROR;

  for (int i = 0; i < 3; i++)
    buf2[i] = IMU_CombineLH_Signed(imu, buf[i * 2], buf[i * 2 + 1]);

  data->accelX = (float)buf2[0] * imu->accelMult;
  data->accelY = (float)buf2[1] * imu->accelMult;
  data->accelZ = (float)buf2[2] * imu->accelMult;

  return IMU_OK;
}

IMU_Status_t IMU_SPI_ReadGyro(IMU_Handler_t *imu, IMU_Data_t *data) {
  uint8_t buf[6];
  int16_t buf2[3];

  if (IMU_SPI_ReadBurst(imu, IMU_REG_GYRO_DATA_X1, buf, 6) != IMU_OK)
    return IMU_ERROR;

  for (int i = 0; i < 3; i++)
    buf2[i] = IMU_CombineLH_Signed(imu, buf[2 * i], buf[2 * i + 1]);

  data->gyroX = (float)buf2[0] * imu->gyroMult;
  data->gyroY = (float)buf2[1] * imu->gyroMult;
  data->gyroZ = (float)buf2[2] * imu->gyroMult;

  return IMU_OK;
}

IMU_Status_t IMU_SPI_ReadTemp(IMU_Handler_t *imu, IMU_Data_t *data) {
  uint8_t buf[2];
  int16_t buf2;

  if (IMU_SPI_ReadBurst(imu, IMU_REG_TEMP_DATA1, buf, 2) != IMU_OK)
    return IMU_ERROR;

  buf2 = IMU_CombineLH_Signed(imu, buf[0], buf[1]);

  data->tempC = (((float)buf2) / 132.48f) + 25.0f;

  return IMU_OK;
}

IMU_Status_t IMU_SPI_GetData(IMU_Handler_t *imu) {
  uint8_t buf[14];
  int16_t buf2[7];

  if (IMU_SPI_ReadBurst(imu, IMU_REG_TEMP_DATA1, buf, 14) != IMU_OK)
    return IMU_ERROR;

  for (int i = 0; i < 7; i++)
    buf2[i] = IMU_CombineLH_Signed(imu, buf[2 * i], buf[2 * i + 1]);

  imu->data.tempC = (((float)buf2[0]) / 132.48f) + 25.0f;
  imu->data.accelX = (float)buf2[1] * imu->accelMult;
  imu->data.accelY = (float)buf2[2] * imu->accelMult;
  imu->data.accelZ = (float)buf2[3] * imu->accelMult;
  imu->data.gyroX = (float)buf2[4] * imu->gyroMult;
  imu->data.gyroY = (float)buf2[5] * imu->gyroMult;
  imu->data.gyroZ = (float)buf2[6] * imu->gyroMult;

  return IMU_OK;
}

IMU_Status_t IMU_SPI_ReadFIFO(IMU_Handler_t *imu, IMU_Data_t *data) {
  uint8_t countBuf[2];
  uint16_t count;
  uint8_t packet[20]; // Max packet size

  IMU_SetRegBank(imu, IMU_BANK_SEL_0); // Ensure Bank 0

  // Read FIFO Count
  if (IMU_SPI_ReadBurst(imu, IMU_REG_FIFO_COUNTH, countBuf, 2) != IMU_OK)
    return IMU_ERROR;

  // Check Endianness for Count
  if (imu->config.intf_config0 & IMU_FIFO_COUNT_ENDIAN_BIG)
    count = (uint16_t)(((uint16_t)countBuf[0] << 8) | countBuf[1]);
  else
    count = (uint16_t)(((uint16_t)countBuf[1] << 8) | countBuf[0]);

  if (count == 0)
    return IMU_OK;

  // Calculate Packet Size based on config
  uint8_t packetSize = 1; // Header
  if (imu->config.fifo_config1 & IMU_FIFO_ACCEL_EN)
    packetSize += 6;
  if (imu->config.fifo_config1 & IMU_FIFO_GYRO_EN)
    packetSize += 6;
  if (imu->config.fifo_config1 & IMU_FIFO_TEMP_EN)
    packetSize += 1;
  if (imu->config.fifo_config1 & IMU_FIFO_TMST_FSYNC_EN)
    packetSize += 2;
  if (imu->config.fifo_config1 & IMU_FIFO_HIRES_EN)
    packetSize += 3;

  // Determine number of packets to drain
  uint16_t numPackets;
  if (imu->config.intf_config0 & IMU_FIFO_COUNT_REC_RECORDS) {
    numPackets = count;
  } else {
    numPackets = count / packetSize;
  }

  if (numPackets == 0)
    return IMU_OK;

  // Drain all available packets — data will contain the most recent sample
  for (uint16_t p = 0; p < numPackets; p++) {
    if (IMU_SPI_ReadBurst(imu, IMU_REG_FIFO_DATA, packet, packetSize) != IMU_OK)
      return IMU_ERROR;

    // Parse Packet
    // Packet format: Header, Accel, Gyro, Temp, Tmst, Hires
    uint8_t idx = 1; // Skip Header

    if (imu->config.fifo_config1 & IMU_FIFO_ACCEL_EN) {
      int16_t ax = IMU_CombineLH_Signed(imu, packet[idx], packet[idx + 1]);
      int16_t ay = IMU_CombineLH_Signed(imu, packet[idx + 2], packet[idx + 3]);
      int16_t az = IMU_CombineLH_Signed(imu, packet[idx + 4], packet[idx + 5]);
      data->accelX = (float)ax * imu->accelMult;
      data->accelY = (float)ay * imu->accelMult;
      data->accelZ = (float)az * imu->accelMult;
      idx += 6;
    }

    if (imu->config.fifo_config1 & IMU_FIFO_GYRO_EN) {
      int16_t gx = IMU_CombineLH_Signed(imu, packet[idx], packet[idx + 1]);
      int16_t gy = IMU_CombineLH_Signed(imu, packet[idx + 2], packet[idx + 3]);
      int16_t gz = IMU_CombineLH_Signed(imu, packet[idx + 4], packet[idx + 5]);
      data->gyroX = (float)gx * imu->gyroMult;
      data->gyroY = (float)gy * imu->gyroMult;
      data->gyroZ = (float)gz * imu->gyroMult;
      idx += 6;
    }

    if (imu->config.fifo_config1 & IMU_FIFO_TEMP_EN) {
      int8_t temp = (int8_t)packet[idx];
      data->tempC = ((float)temp / 2.07f) +
                    25.0f; // Approx conversion for 8-bit? Needs verification.
      // Note: Datasheet for 8-bit temp in FIFO is usually different scale.
      // Assuming standard conversion for now.
      idx += 1;
    }

    // Timestamp and HiRes ignored for now in IMU_Data_t
  }

  return IMU_OK;
}

void IMU_CalculateNotchFilter(IMU_Config_t *config, float freqX, float freqY,
                              float freqZ) {
  uint8_t clkdiv = config->clkdiv & 0x7F; // Mask 7 bits
  if (clkdiv == 0)
    clkdiv = 1; // Prevent division by zero, assume 1 as min

  float fdrv = 19200000.0f / ((float)clkdiv * 10.0f);
  float freqs[3] = {freqX, freqY, freqZ};

  // Reset Static 9 (Bits 5:0 used for NF)
  config->gyro_config_static9 &= 0xC0; // Keep Reserved 7:6

  for (int i = 0; i < 3; i++) {
    float coswz = cosf(2.0f * (float)M_PI * freqs[i] / fdrv);
    int16_t nf_coswz = 0;
    uint8_t nf_coswz_sel = 0;

    if (fabsf(coswz) <= 0.875f) {
      nf_coswz = (int16_t)lroundf(coswz * 256.0f);
      nf_coswz_sel = 0;
    } else {
      nf_coswz_sel = 1;
      if (coswz > 0.875f)
        nf_coswz = (int16_t)lroundf(8.0f * (1.0f - coswz) * 256.0f);
      else
        nf_coswz = (int16_t)lroundf(-8.0f * (1.0f + coswz) * 256.0f);
    }

    if (nf_coswz > 255)
      nf_coswz = 255;
    else if (nf_coswz < -256)
      nf_coswz = -256;

    // Values are 9-bit signed.
    // Low 8 bits go to STATIC6/7/8
    uint8_t lowByte = (uint8_t)(nf_coswz & 0xFF);

    // 9th bit (Sign/MSB usually? Or just bit 8?)
    // nf_coswz is int16. If negative, like -1 (0xFFFF), bit 8 is 1.
    // If 256 (0x0100), bit 8 is 1.
    uint8_t highBit = (uint8_t)((nf_coswz >> 8) & 0x01);

    switch (i) {
    case 0: // X
      config->gyro_config_static6 = lowByte;
      config->gyro_config_static9 |= highBit;             // Bit 0
      config->gyro_config_static9 |= (nf_coswz_sel << 3); // Bit 3
      break;
    case 1: // Y
      config->gyro_config_static7 = lowByte;
      config->gyro_config_static9 |= (highBit << 1);      // Bit 1
      config->gyro_config_static9 |= (nf_coswz_sel << 4); // Bit 4
      break;
    case 2: // Z
      config->gyro_config_static8 = lowByte;
      config->gyro_config_static9 |= (highBit << 2);      // Bit 2
      config->gyro_config_static9 |= (nf_coswz_sel << 5); // Bit 5
      break;
    }
  }
}

void IMU_Config_LoadConfigHeader(IMU_Config_t *config) {
  memset(config, 0, sizeof(IMU_Config_t));

  // Link Bank 0
  config->pwr_mgmt0 = auvConfig.imu.temp_mode | auvConfig.imu.idlemode |
                      auvConfig.imu.gyro_mode | auvConfig.imu.accel_mode;
  config->gyro_config0 = auvConfig.imu.gyro_fs | auvConfig.imu.gyro_odr;
  config->accel_config0 = auvConfig.imu.accel_fs | auvConfig.imu.accel_odr;
  config->gyro_config1 =
      auvConfig.imu.gyro_ui_filt_ord | auvConfig.imu.temp_filt_bw;
  config->accel_config1 =
      auvConfig.imu.accel_ui_filt_ord; // Bit 3 used. Bits 4 (ASC) is 0.
  config->gyro_accel_config0 =
      auvConfig.imu.gyro_ui_filt_bw | auvConfig.imu.accel_ui_filt_bw;

  // DEVICE_CONFIG (0x11)
  // DEVICE_CONFIG (0x11)
  config->device_config = auvConfig.imu.spi_mode;

  // DRIVE_CONFIG (0x13)
  config->drive_config = auvConfig.imu.spi_slew | auvConfig.imu.i2c_slew;

  // INT_CONFIG (0x14)
  config->int_config =
      auvConfig.imu.int1_mode | auvConfig.imu.int1_drive_circuit |
      auvConfig.imu.int1_polarity | auvConfig.imu.int2_mode |
      auvConfig.imu.int2_drive_circuit | auvConfig.imu.int2_polarity;

  // FIFO_CONFIG (0x16)
  config->fifo_config = auvConfig.imu.fifo_mode;

  // INTF_CONFIG0 (0x4C)
  // UI_SIFS_CFG (1:0)
  uint8_t sifs = 0;
  if (auvConfig.imu.interface == 0)
    sifs = IMU_UI_SIFS_CFG_DI_SPI;
  else if (auvConfig.imu.interface == 1)
    sifs = IMU_UI_SIFS_CFG_DI_I2C;
  else
    sifs = 0;

  config->intf_config0 =
      auvConfig.imu.sensor_endian | auvConfig.imu.fifo_count_endian |
      auvConfig.imu.fifo_count_rec | auvConfig.imu.fifo_hold_last_data | sifs;

  // INTF_CONFIG1 (0x4D)
  config->intf_config1 =
      auvConfig.imu.clksel | auvConfig.imu.rtc_mode | auvConfig.imu.accel_lpclk;

  // TMST_CONFIG (0x54)
  config->tmst_config = auvConfig.imu.tmst_en | auvConfig.imu.tmst_fsync_en |
                        auvConfig.imu.tmst_delta_en | auvConfig.imu.tmst_res |
                        auvConfig.imu.tmst_to_regs_en;

  // FIFO_CONFIG1 (0x5F)
  config->fifo_config1 = auvConfig.imu.fifo_packet |
                         auvConfig.imu.fifo_wm_gt_th |
                         auvConfig.imu.fifo_partial_rd;

  // FIFO_CONFIG2, 3 (Watermark)
  // WM is in config->fifo_config2 (Low 8) and fifo_config3 (High 4 bits)
  uint16_t wm = auvConfig.imu.fifo_wm;
  config->fifo_config2 = (uint8_t)(wm & 0xFF);
  config->fifo_config3 = (uint8_t)((wm >> 8) & 0x0F);

  // FSYNC_CONFIG (0x62)
  config->fsync_config = auvConfig.imu.fsync_polarity |
                         auvConfig.imu.fsync_ui_flag_clear_sel |
                         auvConfig.imu.fsync_ui_sel;

  // INT_CONFIG0 (0x63)
  config->int_config0 = auvConfig.imu.int_config0;

  // INT_CONFIG1 (0x64)
  config->int_config1 = auvConfig.imu.int_config1;

  // INT_SOURCE0 (0x65)
  config->int_source0 = auvConfig.imu.int1_src & 0x7F;

  // INT_SOURCE3 (0x68) - INT2 equivalent of SOURCE0
  config->int_source3 = auvConfig.imu.int2_src;

  // INT_SOURCE1, INT_SOURCE4 (WOM/SMD) are not in default config header.
  config->int_source1 = 0;
  config->int_source4 = 0;
  config->int_source6 = 0;
  config->int_source7 = 0;
  config->int_source8 = 0;
  config->int_source9 = 0;

  // USER BANK 1
  // SENSOR_CONFIG0 (0x03) - Axis Disable.
  config->sensor_config0 = auvConfig.imu.sensor_config0;

  // GYRO_CONFIG_STATIC2 (0x0B) - NF & AAF
  config->gyro_config_static2 =
      auvConfig.imu.gyro_nf_en | auvConfig.imu.gyro_aaf_en;

  // GYRO_CONFIG_STATIC3 (0x0C) - AAF DELT
  config->gyro_config_static3 = auvConfig.imu.gyro_aaf_delt;

  // GYRO_CONFIG_STATIC4-8 (NF)
  if (auvConfig.imu.gyro_nf_en == IMU_GYRO_NF_EN) {
    // CLKDIV (0x2A) Bank 3
    config->clkdiv = auvConfig.imu.gyro_nf_clkdiv;
    config->gyro_config_static10 = auvConfig.imu.gyro_nf_bw;
  }

  // USER BANK 2
  // ACCEL_CONFIG_STATIC2 (0x03) - AAF
  config->accel_config_static2 =
      auvConfig.imu.accel_aaf_en | auvConfig.imu.accel_aaf_delt;
  // ACCEL_CONFIG_STATIC3/4 (0x04/0x05) - AAF DELT SQR / BITSHIFT
  config->accel_config_static3 =
      (uint8_t)IMU_ACCEL_AAF_DELTSQR(auvConfig.imu.accel_aaf_delt);
  config->accel_config_static4 =
      (uint8_t)IMU_ACCEL_AAF_BITSHIFT(auvConfig.imu.accel_aaf_delt);

  // GYRO AAF DELT SQR (STATIC4/5)
  config->gyro_config_static4 =
      (uint8_t)IMU_GYRO_AAF_DELTSQR(auvConfig.imu.gyro_aaf_delt);
  config->gyro_config_static5 =
      (uint8_t)IMU_GYRO_AAF_BITSHIFT(auvConfig.imu.gyro_aaf_delt);

  // Bank 4 Options
  // INTF_CONFIG4 (0x7A) - SPI 4/3 wire.
  // `auvConfig.imu.spi_wire` is either 0 (`IMU_SPI_AP_3WIRE`) or 2
  // (`IMU_SPI_AP_4WIRE` is `1<<1`=2). Note: `IMU_SPI_AP_4WIRE` defined as `(1
  // << 1)`.
  config->intf_config4 =
      auvConfig.imu.spi_wire; // Also I3C bus mode?
                              // `auvConfig.imu.i3c_bus_mode` -> bit
                              // 6
  config->intf_config4 |= auvConfig.imu.i3c_bus_mode;

  // INTF_CONFIG5 (0x7B) - Pin 9 function
  config->intf_config5 = auvConfig.imu.pin9_function;

  // INTF_CONFIG6 (0x7C) - I3C Mode
  config->intf_config6 = auvConfig.imu.i3c_mode;

  // APEX / OFFSETS - Not in standard config, zeroed by memset.
}
