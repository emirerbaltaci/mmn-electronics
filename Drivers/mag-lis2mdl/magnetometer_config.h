/*
 * MIT License
 *
 * Copyright (c) 2026 Emir Erbaltacı
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

/*	#############################################
 * 	         	magnetometer_config.h
 * 	      	Configuration file for LIS2MDL
 *	############################################# */

#ifndef INC_MAGNETOMETER_CONFIG_H_
#define INC_MAGNETOMETER_CONFIG_H_

#include "magnetometer.h"

/*	#############################################
 * 						Setup
 *	############################################# */



// #################################### General Options

/*
 * 		MAG_SETUP_INTERFACE				Magnetometer Serial Interface
 *
 * 		SPI								0
 * 		I2C								1
 */
#define MAG_SETUP_INTERFACE				0

/*
 * 		MAG_SETUP_MODE					Magnetometer Mode
 *
 * 		Continuous Measurement			MAG_MODE_CONTINUOUS
 * 		Single Measurement				MAG_MODE_SINGLE
 * 		Idle							MAG_MODE_IDLE
 */
#define MAG_SETUP_MODE					MAG_MODE_CONTINUOUS

/*
 * 		MAG_SETUP_ODR					Magnetometer Output Data Rate
 *
 * 		10Hz							MAG_ODR_10HZ
 * 		20Hz							MAG_ODR_20HZ
 * 		50Hz							MAG_ODR_50HZ
 * 		100Hz							MAG_ODR_100HZ
 */
#define MAG_SETUP_ODR					MAG_ODR_100HZ

/*
 * 		MAG_SETUP_PINFUNC				Behavior of Pin 7 (INT/DRDY/SDO)
 * 										Warning: Interrupts are disabled if SDO is selected
 *
 * 		Interrupt Pin					MAG_INT_ON_PIN_EN
 * 		Data Ready Interrupt Pin		MAG_DRDY_ON_PIN_EN
 * 		SDO (MISO) Pin for 4-wire SPI	MAG_4WSPI_EN
 */
#define MAG_SETUP_PINFUNC				MAG_4WSPI_EN

/*
 * 		MAG_SETUP_LP					Magnetometer Low Power Mode
 *
 * 		Disabled - High Res. Mode		MAG_LP_DIS
 * 		Enabled - Low Power Mode		MAG_LP_EN
 */
#define MAG_SETUP_LP					MAG_LP_DIS

/*
 * 		MAG_SETUP_COMP_TEMP				Magnetometer Temperature Compensation
 *
 * 		Disabled						MAG_COMP_TEMP_DIS
 * 		Enabled							MAG_COMP_TEMP_EN
 */
#define MAG_SETUP_COMP_TEMP				MAG_COMP_TEMP_EN

/*
 * 		MAG_SETUP_LPF					Magnetometer Low Pass Filter
 *
 * 		Disabled						MAG_LPF_DIS
 * 		Enabled (BW = ODR/4)			MAG_LPF_EN
 */
#define MAG_SETUP_LPF					MAG_LPF_DIS

/*
 * 		MAG_SETUP_OFF_CANC				Magnetometer Offset Cancellation
 *
 * 		Disabled						MAG_OFF_CANC_DIS
 * 		Enabled							MAG_OFF_CANC_EN
 */
#define MAG_SETUP_OFF_CANC				MAG_OFF_CANC_DIS

/*
 * 		MAG_SETUP_SET_FREQ				Frequency of the Set Pulse
 *
 * 		ODR/64							MAG_SET_FREQ_ODR_DIV64
 * 		Pulse only at pwr-on post-PD	MAG_SET_FREQ_PD
 */
#define MAG_SETUP_SET_FREQ				MAG_SET_FREQ_ODR_DIV64

/*
 * 		MAG_SETUP_OFF_CANC_ONE_SHOT		Magnetometer Offset Cancellation in Single Measurement Mode
 *
 * 		Disabled						MAG_OFF_CANC_ONE_SHOT_DIS
 * 		Enabled							MAG_OFF_CANC_ONE_SHOT_EN
 */
#define MAG_SETUP_OFF_CANC_ONE_SHOT		MAG_OFF_CANC_ONE_SHOT_DIS

/*
 * 		MAG_SETUP_BLE					Data Endian Selection
 *
 * 		Little Endian					MAG_BLE_DEFAULT
 * 		Big Endian						MAG_BLE_INVERSE
 */
#define MAG_SETUP_BLE					MAG_BLE_DEFAULT

/*
 * 		MAG_SETUP_BDU					If enabled, errors arising from asynchronous reads are avoided.
 *
 * 		Disabled						MAG_BDU_DIS
 * 		Enabled							MAG_BDU_EN
 */
#define MAG_SETUP_BDU					MAG_BDU_EN







// #################################### Interrupt Options


/*
 * 		MAG_SETUP_INT					Interrupts
 * 										Warning: If 4-wire SPI is used, interrupts aren't available as pin 7 is occupied.
 *
 * 		Disabled						MAG_IEN_DIS
 * 		Enabled							MAG_IEN_EN
 */
#define MAG_SETUP_INT					MAG_IEN_DIS

/*
 * 		MAG_SETUP_INT_POLARITY			Interrupt Signal Polarity
 *
 * 		Active Low						MAG_IEA_POLARITY_0
 * 		Active High						MAG_IEA_POLARITY_1
 */
#define MAG_SETUP_INT_POLARITY			MAG_IEA_POLARITY_0

/*
 * 		MAG_SETUP_INT_ON_DATAOFF		Whether interrupt before or after hard-iron data correction
 *
 * 		Before Correction				MAG_INT_ON_DATAOFF_DIS
 * 		After Correction				MAG_INT_ON_DATAOFF_EN
 */
#define MAG_SETUP_INT_ON_DATAOFF		MAG_INT_ON_DATAOFF_EN

/*
 * 		MAG_SETUP_INT_MODE				Interrupt Mode
 *
 * 		Pulsed							MAG_IEL_PULSED
 * 		Latched							MAG_IEL_LATCHED
 */
#define MAG_SETUP_INT_MODE				MAG_IEL_PULSED

/*
 * 		MAG_SETUP_INT_TH				Interrupt Threshold
 * 										Absolute value - negative of this value fires an interrupt as well
 *
 *		x = 1 to 32767					x
 */
#define MAG_SETUP_INT_TH				32767

/*
 * 		MAG_SETUP_INT_SRC				Interrupt Source
 * 										Use bitwise OR ( | ) for multiple selection
 *
 *		None							0
 * 		X-axis							MAG_XIEN_EN
 * 		Y-axis							MAG_YIEN_EN
 * 		Z-axis							MAG_ZIEN_EN
 */
#define MAG_SETUP_INT_SRC				0



#endif /* INC_MAGNETOMETER_CONFIG_H_ */
