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

#include "imu_config.h"
#include <math.h>

static inline void IMU_SPI_Enable(IMU_Handler_t* imu){
	HAL_GPIO_WritePin(imu->pGPIOx, imu->GPIO_PIN_x, GPIO_PIN_RESET);
}

static inline void IMU_SPI_Disable(IMU_Handler_t* imu){
	HAL_GPIO_WritePin(imu->pGPIOx, imu->GPIO_PIN_x, GPIO_PIN_SET);
}

static inline void IMU_SPI_NewCommand(IMU_Handler_t* imu){
	if(!(imu->pGPIOx->ODR & imu->GPIO_PIN_x)){
		IMU_SPI_Disable(imu);
		for(volatile int i = 0; i < 10; i++) __NOP();
	}
	IMU_SPI_Enable(imu);
}

IMU_Status_t IMU_SPI_ReadReg(IMU_Handler_t* imu, uint8_t REG, uint8_t* rxBuffer){
	uint8_t addr = REG | 0x80;
	IMU_SPI_NewCommand(imu);
	if(HAL_SPI_Transmit(imu->pSPIx, &addr, 1, HAL_MAX_DELAY) != HAL_OK){
		IMU_SPI_Disable(imu);
		return IMU_ERROR;
	}
	if(HAL_SPI_Receive(imu->pSPIx, rxBuffer, 1, HAL_MAX_DELAY) != HAL_OK){
		IMU_SPI_Disable(imu);
		return IMU_ERROR;
	}
	IMU_SPI_Disable(imu);
	return IMU_OK;
}

IMU_Status_t IMU_SPI_WriteReg(IMU_Handler_t* imu, uint8_t REG, uint8_t txData){
	uint8_t msg[2] = {REG & 0x7F, txData};
	IMU_SPI_NewCommand(imu);
	if(HAL_SPI_Transmit(imu->pSPIx, msg, 2, HAL_MAX_DELAY) != HAL_OK){
		IMU_SPI_Disable(imu);
		return IMU_ERROR;
	}
	IMU_SPI_Disable(imu);
	return IMU_OK;
}

IMU_Status_t IMU_SPI_ReadBurst(IMU_Handler_t* imu, uint8_t REG, uint8_t* rxBuffer, uint8_t size){
	uint8_t addr = REG | 0x80;
	IMU_SPI_NewCommand(imu);
	if(HAL_SPI_Transmit(imu->pSPIx, &addr, 1, HAL_MAX_DELAY) != HAL_OK){
		IMU_SPI_Disable(imu);
		return IMU_ERROR;
	}
	if(HAL_SPI_Receive(imu->pSPIx, rxBuffer, size, HAL_MAX_DELAY) != HAL_OK){
		IMU_SPI_Disable(imu);
		return IMU_ERROR;
	}
	IMU_SPI_Disable(imu);
	return IMU_OK;
}

IMU_Status_t IMU_SPI_WriteBurst(IMU_Handler_t* imu, uint8_t REG, uint8_t* txBuffer, uint8_t size){
	uint8_t addr = REG & 0x7F;
	IMU_SPI_NewCommand(imu);
	if(HAL_SPI_Transmit(imu->pSPIx, &addr, 1, HAL_MAX_DELAY) != HAL_OK){
		IMU_SPI_Disable(imu);
		return IMU_ERROR;
	}
	if(HAL_SPI_Transmit(imu->pSPIx, txBuffer, size, HAL_MAX_DELAY) != HAL_OK){
		IMU_SPI_Disable(imu);
		return IMU_ERROR;
	}
	IMU_SPI_Disable(imu);
	return IMU_OK;
}

static inline void IMU_MultiplierHelper(IMU_Handler_t* imu, uint8_t ACCEL_UI_FS_SEL, uint8_t GYRO_UI_FS_SEL){
	switch (ACCEL_UI_FS_SEL){
	case IMU_ACCEL_UI_FS_SEL_16G: imu->accelMult = 1.0f / 2048.0f; break;
	case IMU_ACCEL_UI_FS_SEL_8G: imu->accelMult = 1.0f / 4096.0f; break;
	case IMU_ACCEL_UI_FS_SEL_4G: imu->accelMult = 1.0f / 8192.0f; break;
	case IMU_ACCEL_UI_FS_SEL_2G: imu->accelMult = 1.0f / 16384.0f; break;
	default: imu->accelMult = 1.0f / 2048.0f; break;
	}

	switch (GYRO_UI_FS_SEL){
	case IMU_GYRO_UI_FS_SEL_2000DPS: imu->gyroMult = 1.0f / 16.4f; break;
	case IMU_GYRO_UI_FS_SEL_1000DPS: imu->gyroMult = 1.0f / 32.8f; break;
	case IMU_GYRO_UI_FS_SEL_500DPS: imu->gyroMult = 1.0f / 65.5f; break;
	case IMU_GYRO_UI_FS_SEL_250DPS: imu->gyroMult = 1.0f / 131.0f; break;
	case IMU_GYRO_UI_FS_SEL_125DPS: imu->gyroMult = 1.0f / 262.0f; break;
	case IMU_GYRO_UI_FS_SEL_62_5DPS: imu->gyroMult = 1.0f / 524.3f; break;
	case IMU_GYRO_UI_FS_SEL_31_25DPS: imu->gyroMult = 1.0f / 1048.6f; break;
	case IMU_GYRO_UI_FS_SEL_15_625DPS: imu->gyroMult = 1.0f / 2097.2f; break;
	default: imu->gyroMult = 1.0f / 16.4f; break;
	}
}

static inline void IMU_SetRegBank(IMU_Handler_t* imu, uint8_t BANK_SEL_x){
	IMU_SPI_WriteReg(imu, IMU_REG_REG_BANK_SEL, BANK_SEL_x);
}

IMU_Status_t IMU_SPI_Init(IMU_Handler_t* imu){
	uint8_t temp = 0;
	// Register Bank 0
	IMU_SetRegBank(imu, IMU_BANK_SEL_0);

	// Soft Reset, DEVICE_CONFIG and WHO_AM_I
	if(IMU_SPI_WriteReg(imu, IMU_REG_DEVICE_CONFIG, IMU_SOFT_RESET_CONFIG_1) != IMU_OK) return IMU_ERROR;
	HAL_Delay(1);
	if(IMU_SPI_ReadReg(imu, IMU_REG_WHO_AM_I, &temp) != IMU_OK) return IMU_ERROR;
	if(temp != IMU_WHOAMI) return IMU_ERROR;
	if(IMU_SPI_ReadReg(imu, IMU_REG_DEVICE_CONFIG, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xEE;
	if((imu->pSPIx->Instance->CR1 & 0x01) == ((imu->pSPIx->Instance->CR1 & 0x02) >> 1)) temp |= IMU_SPI_MODE_0_3;
	else temp |= IMU_SPI_MODE_1_2;
	if(IMU_SPI_WriteReg(imu, IMU_REG_DEVICE_CONFIG, temp) != IMU_OK) return IMU_ERROR;

	// PWR_MGMT0 - Earlier since clock needs to stabilize
	if(IMU_SPI_ReadReg(imu, IMU_REG_PWR_MGMT0, &temp) != IMU_OK) return IMU_ERROR;
	temp &= (3 << 6);
	temp |= IMU_SETUP_ACCEL_MODE | IMU_SETUP_GYRO_MODE | IMU_SETUP_TEMP_MODE | IMU_SETUP_IDLEMODE;
	if(IMU_SPI_WriteReg(imu, IMU_REG_PWR_MGMT0, temp) != IMU_OK) return IMU_ERROR;

	// DRIVE_CONFIG
	if(IMU_SPI_ReadReg(imu, IMU_REG_DRIVE_CONFIG, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xC0;
	temp |= IMU_SETUP_SPI_SLEW | IMU_SETUP_I2C_SLEW;
	if(IMU_SPI_WriteReg(imu, IMU_REG_DRIVE_CONFIG, temp) != IMU_OK) return IMU_ERROR;

	// INT_CONFIG
	if(IMU_SPI_ReadReg(imu, IMU_REG_INT_CONFIG, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xC0;
	temp |= IMU_SETUP_INT1_POLARITY | IMU_SETUP_INT1_DRIVE_CIRCUIT | IMU_SETUP_INT1_MODE |
			IMU_SETUP_INT2_POLARITY | IMU_SETUP_INT2_DRIVE_CIRCUIT | IMU_SETUP_INT2_MODE;
	if(IMU_SPI_WriteReg(imu, IMU_REG_INT_CONFIG, temp) != IMU_OK) return IMU_ERROR;

	// FIFO_CONFIG
	if(IMU_SPI_ReadReg(imu, IMU_REG_FIFO_CONFIG, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x3F;
	temp |= IMU_SETUP_FIFO_MODE;
	if(IMU_SPI_WriteReg(imu, IMU_REG_FIFO_CONFIG, temp) != IMU_OK) return IMU_ERROR;

	// INTF_CONFIG0
	if(IMU_SPI_ReadReg(imu, IMU_REG_INTF_CONFIG0, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x0C;
#if IMU_SETUP_INTERFACE == 0	// SPI
	temp |= 0x03;
#else	// I2C & I3C
	temp |= 0x02;
#endif
	temp |= IMU_SETUP_SENSOR_ENDIAN | IMU_SETUP_FIFO_COUNT_ENDIAN | IMU_SETUP_FIFO_COUNT_REC | IMU_SETUP_FIFO_HOLD_LAST_DATA;
	if(IMU_SPI_WriteReg(imu, IMU_REG_INTF_CONFIG0, temp) != IMU_OK) return IMU_ERROR;

	// INTF_CONFIG1
	if(IMU_SPI_ReadReg(imu, IMU_REG_INTF_CONFIG1, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xF0;
	temp |= IMU_SETUP_CLKSEL | IMU_SETUP_RTC_MODE | IMU_SETUP_ACCEL_LPCLK;
	if(IMU_SPI_WriteReg(imu, IMU_REG_INTF_CONFIG1, temp) != IMU_OK) return IMU_ERROR;

	// GYRO_CONFIG0 and ACCEL_CONFIG0
	if(IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG0, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x10;
	temp |= IMU_SETUP_GYRO_ODR | IMU_SETUP_GYRO_FS;
	if(IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG0, temp) != IMU_OK) return IMU_ERROR;

	if(IMU_SPI_ReadReg(imu, IMU_REG_ACCEL_CONFIG0, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x10;
	temp |= IMU_SETUP_ACCEL_ODR | IMU_SETUP_ACCEL_FS;
	if(IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_CONFIG0, temp) != IMU_OK) return IMU_ERROR;

	// GYRO_CONFIG1 and ACCEL_CONFIG1
	if(IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG1, &temp) != IMU_OK) return IMU_ERROR;
#if (IMU_SETUP_GYRO_UI_FILT_BW == IMU_GYRO_UI_FILT_BW_DEC2_ODR || IMU_SETUP_GYRO_UI_FILT_BW == IMU_GYRO_UI_FILT_BW_DEC2_ODR8)
	temp &= 0x10;
	temp |= IMU_GYRO_DEC2_M2_ORD_3RD | IMU_SETUP_GYRO_UI_FILT_ORD | IMU_SETUP_TEMP_FILT_BW;
#else
	temp &= 0x13;
	temp |= IMU_SETUP_GYRO_UI_FILT_ORD | IMU_SETUP_TEMP_FILT_BW;
#endif
	if(IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG1, temp) != IMU_OK) return IMU_ERROR;

	if(IMU_SPI_ReadReg(imu, IMU_REG_ACCEL_CONFIG1, &temp) != IMU_OK) return IMU_ERROR;
#if (IMU_SETUP_ACCEL_UI_FILT_BW == IMU_ACCEL_UI_FILT_BW_LN_DEC2_ODR || IMU_SETUP_ACCEL_UI_FILT_BW == IMU_ACCEL_UI_FILT_BW_LN_DEC2_ODR8)
	temp &= 0xE1;
	temp |= IMU_ACCEL_DEC2_M2_ORD_3RD | IMU_SETUP_ACCEL_UI_FILT_ORD;
#else
	temp &= 0xE7;
	temp |= IMU_SETUP_ACCEL_UI_FILT_ORD;
#endif
	if(IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_CONFIG1, temp) != IMU_OK) return IMU_ERROR;

	// GYRO_ACCEL_CONFIG0
	temp = IMU_SETUP_GYRO_UI_FILT_BW | IMU_SETUP_ACCEL_UI_FILT_BW;
	if(IMU_SPI_WriteReg(imu, IMU_REG_GYRO_ACCEL_CONFIG0, temp) != IMU_OK) return IMU_ERROR;

	// TMST_CONFIG
	if(IMU_SPI_ReadReg(imu, IMU_REG_TMST_CONFIG, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xE0;
	temp |= IMU_SETUP_TMST_EN | IMU_SETUP_TMST_FSYNC_EN | IMU_SETUP_TMST_DELTA_EN | IMU_SETUP_TMST_RES | IMU_SETUP_TMST_TO_REGS_EN;
	if(IMU_SPI_WriteReg(imu, IMU_REG_TMST_CONFIG, temp) != IMU_OK) return IMU_ERROR;

	// FIFO_CONFIG1
	if(IMU_SPI_ReadReg(imu, IMU_REG_FIFO_CONFIG1, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x80;
	temp |= IMU_SETUP_FIFO_PACKET | IMU_FIFO_TEMP_EN | IMU_SETUP_FIFO_WM_GT_TH | IMU_SETUP_FIFO_PARTIAL_RD;
	if(IMU_SPI_WriteReg(imu, IMU_REG_FIFO_CONFIG1, temp) != IMU_OK) return IMU_ERROR;

	// FIFO_CONFIG2 and FIFO_CONFIG3
#if (IMU_SETUP_FIFO_WM >= 1 && IMU_SETUP_FIFO_WM <= 4095)
	temp = (uint8_t)(IMU_SETUP_FIFO_WM & 0xFF);
	if(IMU_SPI_WriteReg(imu, IMU_REG_FIFO_CONFIG2, temp) != IMU_OK) return IMU_ERROR;

	if(IMU_SPI_ReadReg(imu, IMU_REG_FIFO_CONFIG3, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xF0;
	temp |= (uint8_t)((IMU_SETUP_FIFO_WM >> 8) & 0x0F);
	if(IMU_SPI_WriteReg(imu, IMU_REG_FIFO_CONFIG3, temp) != IMU_OK) return IMU_ERROR;
#else
	return IMU_INVALID_CONFIG;
#endif

	// FSYNC_CONFIG
	if(IMU_SPI_ReadReg(imu, IMU_REG_FSYNC_CONFIG, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x8C;
	temp |= IMU_SETUP_FSYNC_POLARITY | IMU_SETUP_FSYNC_UI_FLAG_CLEAR_SEL | IMU_SETUP_FSYNC_UI_SEL;
	if(IMU_SPI_WriteReg(imu, IMU_REG_FSYNC_CONFIG, temp) != IMU_OK) return IMU_ERROR;

	// SIGNAL_PATH_RESET - Flush FIFO before enabling interrupts
	if(IMU_SPI_ReadReg(imu, IMU_REG_SIGNAL_PATH_RESET, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x91;
	temp |= IMU_FIFO_FLUSH;
	if(IMU_SPI_WriteReg(imu, IMU_REG_SIGNAL_PATH_RESET, temp) != IMU_OK) return IMU_ERROR;

	// INT_CONFIG0
	if(IMU_SPI_ReadReg(imu, IMU_REG_INT_CONFIG0, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xC0;
	temp |= IMU_SETUP_INT_CONFIG0;
	if(IMU_SPI_WriteReg(imu, IMU_REG_INT_CONFIG0, temp) != IMU_OK) return IMU_ERROR;

	// INT_CONFIG1
	if(IMU_SPI_ReadReg(imu, IMU_REG_INT_CONFIG1, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x8F;
#if (IMU_SETUP_ACCEL_ODR <= IMU_ACCEL_ODR_4KHZ || IMU_SETUP_GYRO_ODR <= IMU_GYRO_ODR_4KHZ)
	temp |= (3 << 5);
#endif
	temp |= IMU_INT_ASYNC_RESET;
	if(IMU_SPI_WriteReg(imu, IMU_REG_INT_CONFIG1, temp) != IMU_OK) return IMU_ERROR;

	// INT_SOURCE0 and INT_SOURCE_3
	if(IMU_SPI_ReadReg(imu, IMU_REG_INT_SOURCE0, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x80;
	temp |= IMU_RESET_DONE_INT1_EN | IMU_SETUP_INT1_SRC;
	if(IMU_SPI_WriteReg(imu, IMU_REG_INT_SOURCE0, temp) != IMU_OK) return IMU_ERROR;

	if(IMU_SPI_ReadReg(imu, IMU_REG_INT_SOURCE3, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x80;
	temp |= IMU_RESET_DONE_INT2_EN | IMU_SETUP_INT2_SRC;
	if(IMU_SPI_WriteReg(imu, IMU_REG_INT_SOURCE3, temp) != IMU_OK) return IMU_ERROR;

	// SELF_TEST_CONFIG
	if(IMU_SPI_ReadReg(imu, IMU_REG_SELF_TEST_CONFIG, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x80;
	temp |= 0x3F;	// Self-test on all axes when conducted
	if(IMU_SPI_WriteReg(imu, IMU_REG_SELF_TEST_CONFIG, temp) != IMU_OK) return IMU_ERROR;

	IMU_SetRegBank(imu, IMU_BANK_SEL_1);

	// SENSOR_CONFIG0
	if(IMU_SPI_ReadReg(imu, IMU_REG_SENSOR_CONFIG0, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xC0;
	temp |= 0;	// Enable all axes
	if(IMU_SPI_WriteReg(imu, IMU_REG_SENSOR_CONFIG0, temp) != IMU_OK) return IMU_ERROR;

	// CLKDIV before AAF Config
	IMU_SetRegBank(imu, IMU_BANK_SEL_3);
	if(IMU_SPI_ReadReg(imu, IMU_REG_CLKDIV, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x80;
	temp |= IMU_SETUP_GYRO_NF_CLKDIV;
	if(IMU_SPI_WriteReg(imu, IMU_REG_CLKDIV, temp) != IMU_OK) return IMU_ERROR;
	IMU_SetRegBank(imu, IMU_BANK_SEL_1);

	// GYRO_CONFIG_STATIC2 to GYRO_CONFIG_STATIC_10
	if(IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG_STATIC2, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xFC;
	temp |= IMU_SETUP_GYRO_AAF_EN | IMU_SETUP_GYRO_NF_EN;
	if(IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC2, temp) != IMU_OK) return IMU_ERROR;

	if(IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG_STATIC3, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xC0;
	temp |= IMU_SETUP_GYRO_AAF_DELT;
	if(IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC3, temp) != IMU_OK) return IMU_ERROR;

	temp = (uint8_t)(IMU_GYRO_AAF_DELTSQR(IMU_SETUP_GYRO_AAF_DELT) & 0xFF);
	if(IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC4, temp) != IMU_OK) return IMU_ERROR;

	temp = (uint8_t)((IMU_GYRO_AAF_DELTSQR(IMU_SETUP_GYRO_AAF_DELT) >> 8) & 0x0F) | IMU_GYRO_AAF_BITSHIFT(IMU_SETUP_GYRO_AAF_DELT);
	if(IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC5, temp) != IMU_OK) return IMU_ERROR;

	uint8_t temp2 = 0;
	if(IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG_STATIC9, &temp2) != IMU_OK) return IMU_ERROR;
	temp2 &= 0xC0;
	float fdrv = 19200000.0f / ((float)IMU_SETUP_GYRO_NF_CLKDIV * 10.0f);
	for(int i = 0; i < 3; i++){
		uint16_t freq;
		uint8_t reg;
		switch (i) {
		case 0:
			freq = IMU_SETUP_GYRO_NF_X_FREQ;
			reg = IMU_REG_GYRO_CONFIG_STATIC6;
			break;
		case 1:
			freq = IMU_SETUP_GYRO_NF_Y_FREQ;
			reg = IMU_REG_GYRO_CONFIG_STATIC7;
			break;
		case 2:
			freq = IMU_SETUP_GYRO_NF_Z_FREQ;
			reg = IMU_REG_GYRO_CONFIG_STATIC8;
			break;
		default:
			return IMU_INVALID_CONFIG;
			break;
		}
		float coswz = cosf(2.0f * (float)M_PI * freq / fdrv);
		int16_t nf_coswz = 0;
		uint8_t nf_coswz_sel = 0;
		if(fabsf(coswz) <= 0.875f){
			nf_coswz = (int16_t)lroundf(coswz * 256.0f);
			nf_coswz_sel = 0;
		}
		else{
			nf_coswz_sel = 1;
			if(coswz > 0.875f) nf_coswz = (int16_t)lroundf(8.0f * (1.0f - coswz) * 256.0f);
			else nf_coswz = (int16_t)lroundf(-8.0f * (1.0f + coswz) * 256.0f);
		}
		if(nf_coswz > 255) nf_coswz = 255;
		else if(nf_coswz < -256) nf_coswz = -256;
		temp = nf_coswz & 0xFF;
		if(IMU_SPI_WriteReg(imu, reg, temp) != IMU_OK) return IMU_ERROR;
		switch (i) {
		case 0:
			temp2 |= (uint8_t)((nf_coswz >> 8) & 0x01) | (nf_coswz_sel << 3);
			break;
		case 1:
			temp2 |= ((uint8_t)((nf_coswz >> 8) & 0x01) << 1) | (nf_coswz_sel << 4);
			break;
		case 2:
			temp2 |= ((uint8_t)((nf_coswz >> 8) & 0x01) << 2) | (nf_coswz_sel << 5);
			break;
		default:
			return IMU_INVALID_CONFIG;
			break;
		}
	}
	if(IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC9, temp2) != IMU_OK) return IMU_ERROR;

	if(IMU_SPI_ReadReg(imu, IMU_REG_GYRO_CONFIG_STATIC10, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x8F;
	temp |= IMU_SETUP_GYRO_NF_BW;
	if(IMU_SPI_WriteReg(imu, IMU_REG_GYRO_CONFIG_STATIC10, temp) != IMU_OK) return IMU_ERROR;

	// INTF_CONFIG4
	if(IMU_SPI_ReadReg(imu, IMU_REG_INTF_CONFIG4, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xBD;
	temp |= IMU_SETUP_SPI_WIRE | IMU_SETUP_I3C_BUS_MODE;
	if(IMU_SPI_WriteReg(imu, IMU_REG_INTF_CONFIG4, temp) != IMU_OK) return IMU_ERROR;

	// INTF_CONFIG5
	if(IMU_SPI_ReadReg(imu, IMU_REG_INTF_CONFIG5, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0xF9;
	temp |= IMU_SETUP_PIN9_FUNCTION;
	if(IMU_SPI_WriteReg(imu, IMU_REG_INTF_CONFIG5, temp) != IMU_OK) return IMU_ERROR;

	// INTF_CONFIG6 (Implement Later)

	IMU_SetRegBank(imu, IMU_BANK_SEL_2);
	if(IMU_SPI_ReadReg(imu, IMU_REG_ACCEL_CONFIG_STATIC2, &temp) != IMU_OK) return IMU_ERROR;
	temp &= 0x80;
	temp |= IMU_SETUP_ACCEL_AAF_DELT | IMU_SETUP_ACCEL_AAF_EN;
	if(IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_CONFIG_STATIC2, temp) != IMU_OK) return IMU_ERROR;

	temp = (uint8_t)(IMU_ACCEL_AAF_DELTSQR(IMU_SETUP_ACCEL_AAF_DELT) & 0xFF);
	if(IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_CONFIG_STATIC3, temp) != IMU_OK) return IMU_ERROR;

	temp = (uint8_t)((IMU_ACCEL_AAF_DELTSQR(IMU_SETUP_ACCEL_AAF_DELT) >> 8) & 0x0F) | IMU_ACCEL_AAF_BITSHIFT(IMU_SETUP_ACCEL_AAF_DELT);
	if(IMU_SPI_WriteReg(imu, IMU_REG_ACCEL_CONFIG_STATIC4, temp) != IMU_OK) return IMU_ERROR;

	// APEX CONFIG NOT IMPLEMENTED YET, SKIPPED FOR NOW - NO NEED FOR APEX IN AUV
	// INT_SOURCEx, x = 1, 4, 6, 7, 8, 9, 10 ARE ALSO SKIPPED

	IMU_SetRegBank(imu, IMU_BANK_SEL_0);

	IMU_MultiplierHelper(imu, IMU_SETUP_ACCEL_FS, IMU_SETUP_GYRO_FS);

	return IMU_OK;
}

static inline int16_t IMU_CombineLH_Signed(uint8_t higherBits, uint8_t lowerBits){
	if(IMU_SETUP_SENSOR_ENDIAN == IMU_SENSOR_DATA_ENDIAN_BIG) return (int16_t)((((uint16_t)higherBits) << 8) | lowerBits);
	else return (int16_t)((((uint16_t)lowerBits) << 8) | higherBits);
}

static inline uint16_t IMU_CombineLH_Unsigned(uint8_t higherBits, uint8_t lowerBits){
	if(IMU_SETUP_SENSOR_ENDIAN == IMU_SENSOR_DATA_ENDIAN_BIG) return (uint16_t)((((uint16_t)higherBits) << 8) | lowerBits);
	else return (uint16_t)((((uint16_t)lowerBits) << 8) | higherBits);
}

IMU_Status_t IMU_SPI_ReadAccel(IMU_Handler_t* imu, IMU_Data_t* data){
	uint8_t buf[6];
	int16_t buf2[3];

	if(IMU_SPI_ReadBurst(imu, IMU_REG_ACCEL_DATA_X1, buf, 6) != IMU_OK) return IMU_ERROR;

	for(int i = 0; i < 3; i++) buf2[i] = IMU_CombineLH_Signed(buf[i*2], buf[i*2 + 1]);

	data->accelX = (float)buf2[0] * imu->accelMult;
	data->accelY = (float)buf2[1] * imu->accelMult;
	data->accelZ = (float)buf2[2] * imu->accelMult;

	return IMU_OK;
}

IMU_Status_t IMU_SPI_ReadGyro(IMU_Handler_t* imu, IMU_Data_t* data){
	uint8_t buf[6];
	int16_t buf2[3];

	if(IMU_SPI_ReadBurst(imu, IMU_REG_GYRO_DATA_X1, buf, 6) != IMU_OK) return IMU_ERROR;

	for(int i = 0; i < 3; i++) buf2[i] = IMU_CombineLH_Signed(buf[2*i], buf[2*i + 1]);

	data->gyroX = (float)buf2[0] * imu->gyroMult;
	data->gyroY = (float)buf2[1] * imu->gyroMult;
	data->gyroZ = (float)buf2[2] * imu->gyroMult;

	return IMU_OK;
}

IMU_Status_t IMU_SPI_ReadTemp(IMU_Handler_t* imu, IMU_Data_t* data){
	uint8_t buf[2];
	int16_t buf2;

	if(IMU_SPI_ReadBurst(imu, IMU_REG_TEMP_DATA1, buf, 2) != IMU_OK) return IMU_ERROR;

	buf2 = IMU_CombineLH_Signed(buf[0], buf[1]);

	data->tempC = (((float)buf2) / 132.48f) + 25.0f;

	return IMU_OK;
}

IMU_Status_t IMU_SPI_GetData(IMU_Handler_t* imu, IMU_Data_t* data){
	uint8_t buf[14];
	int16_t buf2[7];

	if(IMU_SPI_ReadBurst(imu, IMU_REG_TEMP_DATA1, buf, 14) != IMU_OK) return IMU_ERROR;

	for(int i = 0; i < 7; i++) buf2[i] = IMU_CombineLH_Signed(buf[2*i], buf[2*i + 1]);

	data->tempC = (((float)buf2[0]) / 132.48f) + 25.0f;
	data->accelX = (float)buf2[1] * imu->accelMult;
	data->accelY = (float)buf2[2] * imu->accelMult;
	data->accelZ = (float)buf2[3] * imu->accelMult;
	data->gyroX = (float)buf2[4] * imu->gyroMult;
	data->gyroY = (float)buf2[5] * imu->gyroMult;
	data->gyroZ = (float)buf2[6] * imu->gyroMult;

	return IMU_OK;
}
