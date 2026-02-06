/*
 * MIT License
 *
 * Copyright (c) 2026 MM Nautronics Team
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

#ifndef INC_MAGNETOMETER_H_
#define INC_MAGNETOMETER_H_

#include "stm32g4xx_hal.h"

#define MAG_I2C_ADDR		0x1E

typedef enum{
	MAG_OK = 0,
	MAG_ERROR = 1,
	MAG_BUSY = 2,
	MAG_TIMEOUT = 3,
	MAG_INVALID_CONFIG = 4
}MAG_Status_t;

#define MAG_REG_OFFSET_X_L 		0x45
#define MAG_REG_OFFSET_X_H 		0x46
#define MAG_REG_OFFSET_Y_L 		0x47
#define MAG_REG_OFFSET_Y_H 		0x48
#define MAG_REG_OFFSET_Z_L 		0x49
#define MAG_REG_OFFSET_Z_H 		0x4A
#define MAG_REG_WHO_AM_I 		0x4F
#define MAG_REG_CFG_REG_A 		0x60
#define MAG_REG_CFG_REG_B 		0x61
#define MAG_REG_CFG_REG_C 		0x62
#define MAG_REG_INT_CTRL_REG 	0x63
#define MAG_REG_INT_SOURCE 		0x64
#define MAG_REG_INT_THS_L 		0x65
#define MAG_REG_INT_THS_H		0x66
#define MAG_REG_STATUS 			0x67
#define MAG_REG_OUTX_L 			0x68
#define MAG_REG_OUTX_H 			0x69
#define MAG_REG_OUTY_L 			0x6A
#define MAG_REG_OUTY_H 			0x6B
#define MAG_REG_OUTZ_L 			0x6C
#define MAG_REG_OUTZ_H 			0x6D
#define MAG_REG_TEMP_OUT_L 		0x6E
#define MAG_REG_TEMP_OUT_H 		0x6F



// WHO_AM_I	(0x4F)
#define MAG_WHOAMI				0x40

// CFG_REG_A	(0x60)
#define MAG_MODE_CONTINUOUS				0
#define MAG_MODE_SINGLE 				1
#define MAG_MODE_IDLE 					2
#define MAG_ODR_10HZ					0
#define MAG_ODR_20HZ 					(1 << 2)
#define MAG_ODR_50HZ 					(2 << 2)
#define MAG_ODR_100HZ 					(3 << 2)
#define MAG_LP_DIS 						0
#define MAG_LP_EN 						(1 << 4)
#define MAG_SOFT_RST					(1 << 5)
#define MAG_REBOOT						(1 << 6)
#define MAG_COMP_TEMP_DIS				0
#define MAG_COMP_TEMP_EN				(1 << 7)

// CFG_REG_B	(0x61)
#define MAG_LPF_DIS 					0
#define MAG_LPF_EN 						1
#define MAG_OFF_CANC_DIS 				0
#define MAG_OFF_CANC_EN 				(1 << 1)
#define MAG_SET_FREQ_ODR_DIV64			0
#define MAG_SET_FREQ_PD					(1 << 2)
#define MAG_INT_ON_DATAOFF_DIS			0
#define MAG_INT_ON_DATAOFF_EN			(1 << 3)
#define MAG_OFF_CANC_ONE_SHOT_DIS		0
#define MAG_OFF_CANC_ONE_SHOT_EN		(1 << 4)

// CFG_REG_C	(0x62)
#define MAG_DRDY_ON_PIN_EN				1
#define MAG_SELF_TEST_DIS				0
#define MAG_SELF_TEST_EN				(1 << 1)
#define MAG_4WSPI_EN					(1 << 2)
#define MAG_BLE_DEFAULT					0
#define MAG_BLE_INVERSE					(1 << 3)
#define MAG_BDU_DIS						0
#define MAG_BDU_EN						(1 << 4)
#define MAG_I2C_EN						0
#define MAG_I2C_DIS						(1 << 5)
#define MAG_INT_ON_PIN_EN				(1 << 6)

// INT_CTRL_REG	(0x63)
#define MAG_IEN_DIS						0
#define MAG_IEN_EN						1
#define MAG_IEL_PULSED					0
#define MAG_IEL_LATCHED					(1 << 1)
#define MAG_IEA_POLARITY_0				0
#define MAG_IEA_POLARITY_1				(1 << 2)
#define MAG_ZIEN_DIS					0
#define MAG_ZIEN_EN						(1 << 5)
#define MAG_YIEN_DIS					0
#define MAG_YIEN_EN						(1 << 6)
#define MAG_XIEN_DIS					0
#define MAG_XIEN_EN						(1 << 7)

// INT_SOURCE_REG	(0x64)
// INT_THS_L_REG	(0x65)
// INT_THS_H_REG	(0x66)
// STATUS_REG	(0x67)
// OUTX_L_REG	(0x68)
// OUTX_H_REG	(0x69)
// OUTY_L_REG	(0x6A)
// OUTY_H_REG	(0x6B)
// OUTZ_L_REG	(0x6C)
// OUTZ_H_REG	(0x6D)
// TEMP_OUT_L_REG (0x6E)
// TEMP_OUT_H_REG (0x6F)

typedef struct{

}Mag_Config_t;

typedef struct{
	SPI_HandleTypeDef* pSPIx;
	GPIO_TypeDef* pGPIOx;
	uint16_t GPIO_PIN_x;
}MAG_Handler_t;

typedef struct{
	float magX;
	float magY;
	float magZ;
	float tempC;
}MAG_Data_t;


#endif /* INC_MAGNETOMETER_H_ */
