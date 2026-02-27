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

#include "magnetometer_config.h"

static inline void MAG_SPI_Enable(MAG_Handler_t* mag){
	HAL_GPIO_WritePin(mag->pGPIOx, mag->GPIO_PIN_x, GPIO_PIN_RESET);
}

static inline void MAG_SPI_Disable(MAG_Handler_t* mag){
	HAL_GPIO_WritePin(mag->pGPIOx, mag->GPIO_PIN_x, GPIO_PIN_SET);
}

static inline void MAG_SPI_NewCommand(MAG_Handler_t* mag){
	if(!(mag->pGPIOx->ODR & mag->GPIO_PIN_x)){
		MAG_SPI_Disable(mag);
		for(volatile int i = 0; i < 10; i++) __NOP();
	}
	MAG_SPI_Enable(mag);
}

MAG_Status_t MAG_SPI_ReadReg(MAG_Handler_t* mag, uint8_t REG, uint8_t* rxBuffer){
	uint8_t addr = REG | 0x80;
	MAG_SPI_NewCommand(mag);
	if(HAL_SPI_Transmit(mag->pSPIx, &addr, 1, HAL_MAX_DELAY) != HAL_OK){
		MAG_SPI_Disable(mag);
		return MAG_ERROR;
	}
	if(HAL_SPI_Receive(mag->pSPIx, rxBuffer, 1, HAL_MAX_DELAY) != HAL_OK){
		MAG_SPI_Disable(mag);
		return MAG_ERROR;
	}
	MAG_SPI_Disable(mag);
	return MAG_OK;
}

MAG_Status_t MAG_SPI_WriteReg(MAG_Handler_t* mag, uint8_t REG, uint8_t txData){
	uint8_t msg[2] = {REG & 0x7F, txData};
	MAG_SPI_NewCommand(mag);
	if(HAL_SPI_Transmit(mag->pSPIx, msg, 2, HAL_MAX_DELAY) != HAL_OK){
		MAG_SPI_Disable(mag);
		return MAG_ERROR;
	}
	MAG_SPI_Disable(mag);
	return MAG_OK;
}

MAG_Status_t MAG_SPI_ReadBurst(MAG_Handler_t* mag, uint8_t REG, uint8_t* rxBuffer, uint8_t size){
	uint8_t addr = REG | 0x80;
	MAG_SPI_NewCommand(mag);
	if(HAL_SPI_Transmit(mag->pSPIx, &addr, 1, HAL_MAX_DELAY) != HAL_OK){
		MAG_SPI_Disable(mag);
		return MAG_ERROR;
	}
	if(HAL_SPI_Receive(mag->pSPIx, rxBuffer, size, HAL_MAX_DELAY) != HAL_OK){
		MAG_SPI_Disable(mag);
		return MAG_ERROR;
	}
	MAG_SPI_Disable(mag);
	return MAG_OK;
}

MAG_Status_t MAG_SPI_WriteBurst(MAG_Handler_t* mag, uint8_t REG, uint8_t* txBuffer, uint8_t size){
	uint8_t addr = REG & 0x7F;
	MAG_SPI_NewCommand(mag);
	if(HAL_SPI_Transmit(mag->pSPIx, &addr, 1, HAL_MAX_DELAY) != HAL_OK){
		MAG_SPI_Disable(mag);
		return MAG_ERROR;
	}
	if(HAL_SPI_Transmit(mag->pSPIx, txBuffer, size, HAL_MAX_DELAY) != HAL_OK){
		MAG_SPI_Disable(mag);
		return MAG_ERROR;
	}
	MAG_SPI_Disable(mag);
	return MAG_OK;
}

MAG_Status_t MAG_SPI_Init(MAG_Handler_t* mag){
	uint8_t temp = MAG_REBOOT;
	if(MAG_SPI_WriteReg(mag, MAG_REG_CFG_REG_A, temp) != MAG_OK) return MAG_ERROR;
	
	HAL_Delay(1);
	
	temp = MAG_SOFT_RST;
	if(MAG_SPI_WriteReg(mag, MAG_REG_CFG_REG_A, temp) != MAG_OK) return MAG_ERROR;
	
	HAL_Delay(1);
	
	if(MAG_SPI_ReadReg(mag, MAG_REG_WHO_AM_I, &temp) != MAG_OK) return MAG_ERROR;
	if(temp != MAG_WHOAMI) return MAG_ERROR;

	/* Populate Config Struct from Macros */
	mag->config.cfg_reg_a 		= MAG_SETUP_COMP_TEMP | MAG_SETUP_LP | MAG_SETUP_ODR | MAG_SETUP_MODE;
	mag->config.cfg_reg_b 		= MAG_SETUP_OFF_CANC_ONE_SHOT | MAG_SETUP_INT_ON_DATAOFF | MAG_SETUP_SET_FREQ | MAG_SETUP_OFF_CANC | MAG_SETUP_LPF;
	mag->config.cfg_reg_c 		= MAG_SETUP_PINFUNC | MAG_SETUP_BDU | MAG_SETUP_BLE;
	if(MAG_SETUP_INTERFACE == 0) mag->config.cfg_reg_c |= MAG_I2C_DIS;	// Disable I2C if SPI selected
	
	mag->config.int_ctrl_reg 	= MAG_SETUP_INT_SRC | MAG_SETUP_INT | MAG_SETUP_INT_POLARITY | MAG_SETUP_INT_MODE;
	mag->config.int_ths 		= MAG_SETUP_INT_TH;

	/* Write Configuration to Registers */
	if(MAG_SPI_WriteReg(mag, MAG_REG_CFG_REG_A, mag->config.cfg_reg_a) != MAG_OK) return MAG_ERROR;
	if(MAG_SPI_WriteReg(mag, MAG_REG_CFG_REG_B, mag->config.cfg_reg_b) != MAG_OK) return MAG_ERROR;
	if(MAG_SPI_WriteReg(mag, MAG_REG_CFG_REG_C, mag->config.cfg_reg_c) != MAG_OK) return MAG_ERROR;
	if(MAG_SPI_WriteReg(mag, MAG_REG_INT_CTRL_REG, mag->config.int_ctrl_reg) != MAG_OK) return MAG_ERROR;
	
	// Write Threshold (Low and High bytes)
	if(MAG_SPI_WriteReg(mag, MAG_REG_INT_THS_L, (uint8_t)(mag->config.int_ths & 0xFF)) != MAG_OK) return MAG_ERROR;
	if(MAG_SPI_WriteReg(mag, MAG_REG_INT_THS_H, (uint8_t)((mag->config.int_ths >> 8) & 0xFF)) != MAG_OK) return MAG_ERROR;

	return MAG_OK;
}

static inline int16_t MAG_CombineLH_Signed(uint8_t higherBits, uint8_t lowerBits){
	if(MAG_SETUP_BLE == 0) return (int16_t)((((uint16_t)higherBits) << 8) | lowerBits);
	else return (int16_t)((((uint16_t)lowerBits) << 8) | higherBits);
}

static inline uint16_t MAG_CombineLH_Unsigned(uint8_t higherBits, uint8_t lowerBits){
	if(MAG_SETUP_BLE == 0) return (uint16_t)((((uint16_t)higherBits) << 8) | lowerBits);
	else return (uint16_t)((((uint16_t)lowerBits) << 8) | higherBits);
}

MAG_Status_t MAG_SPI_GetData(MAG_Handler_t* mag){
	uint8_t buf[8];
	int16_t buf2[4];

	if(MAG_SPI_ReadBurst(mag, MAG_REG_OUTX_L, buf, 8) != MAG_OK) return MAG_ERROR;

	for(int i = 0; i < 4; i++) buf2[i] = MAG_CombineLH_Signed(buf[2*i + 1], buf[2*i]);

	mag->data.magX = (float)buf2[0] * 1.5f;
	mag->data.magY = (float)buf2[1] * 1.5f;
	mag->data.magZ = (float)buf2[2] * 1.5f;
	mag->data.tempC = 25.0f + (float)buf2[3] * 0.125f;

	return MAG_OK;
}
