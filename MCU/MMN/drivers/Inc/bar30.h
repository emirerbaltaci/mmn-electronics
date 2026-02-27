/*
 * bar30.h
 *
 *  Created on: 26 Şub 2026
 *      Author: Emir
 */

#ifndef DRIVERS_INC_BAR30_H_
#define DRIVERS_INC_BAR30_H_

#include "stm32g4xx_hal.h"

typedef struct{
	uint8_t i2cAddr;
	float density;
	float factor;
}BAR30_Config_t;

typedef struct{

}BAR30_Data_t;

typedef struct{
	BAR30_Config_t config;
	I2C_HandleTypeDef *pI2Cx;
	float depth;
}BAR30_Handler_t;

void BAR30_GetData(BAR30_Handler_t *bar);

#endif /* DRIVERS_INC_BAR30_H_ */
