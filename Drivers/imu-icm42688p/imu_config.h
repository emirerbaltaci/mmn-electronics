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

/*	#############################################
 * 	         		 imu_config.h
 * 	      Configuration file for ICM-42688-P
 *	############################################# */

#ifndef IMU_CONFIG_H_
#define IMU_CONFIG_H_

#include "imu.h"

/*	#############################################
 * 						Setup
 *	############################################# */



// #################################### General Options

/*
 * 		IMU_SETUP_INTERFACE				Serial Interface Selection
 *
 *		SPI								0
 * 		I2C								1
 * 		I3C								2
 */
#define IMU_SETUP_INTERFACE				0

/*
 * 		IMU_SETUP_SPI_MODE				SPI Mode Selection
 *
 * 		Mode 0 or 3						IMU_SPI_MODE_0_3
 * 		Mode 1 or 2						IMU_SPI_MODE_1_2
 */
#define IMU_SETUP_SPI_MODE				IMU_SPI_MODE_0_3

/*
 * 		IMU_SETUP_SPI_WIRE				SPI Wire Count
 *
 * 		3 wires							IMU_SPI_AP_3WIRE
 * 		4 wires							IMU_SPI_AP_4WIRE
 */
#define IMU_SETUP_SPI_WIRE				IMU_SPI_AP_4WIRE

/*
 * 		IMU_SETUP_I3C_BUS_MODE			I3C BUS Mode
 *
 * 		I2C + I3C						IMU_I3C_BUS_MODE_I2C
 * 		I3C Only						IMU_I3C_BUS_MODE_I3C
 */
#define IMU_SETUP_I3C_BUS_MODE			IMU_I3C_BUS_MODE_I2C

/*
 * 		IMU_SETUP_I3C_MODE				I3C Mode
 *
 * 		SDR								IMU_I3C_SDR_EN
 * 		SDR + DDR						IMU_I3C_DDR_EN
 */
#define IMU_SETUP_I3C_MODE				IMU_I3C_SDR_EN

/*
 * 		IMU_SETUP_CLKSEL				Clock Source Selection
 *
 * 		Internal RC Oscillator			IMU_CLKSEL_INTERNAL
 * 		PLL if available				IMU_CLKSEL_PLL
 * 		Disable all clocks				IMU_CLKSEL_DISABLE
 */
#define IMU_SETUP_CLKSEL				IMU_CLKSEL_PLL

/*
 * 		IMU_SETUP_IDLEMODE				Normally, RC oscillator is powered off if accel&gyro are disabled
 * 										Enable this mode to power it even if accel&gyro are disabled
 *
 * 		Disabled						IMU_IDLE_DIS
 * 		Enabled							IMU_IDLE_EN
 */
#define IMU_SETUP_IDLEMODE				IMU_IDLE_DIS

/*
 * 		IMU_SETUP_PIN9_FUNCTION			Function of Pin 9
 *
 * 		INT2 Pin						IMU_PIN9_FUNCTION_INT2
 * 		FSYNC Pin						IMU_PIN9_FUNCTION_FSYNC
 * 		CLKIN Pin						IMU_PIN9_FUNCTION_CLKIN
 */
#define IMU_SETUP_PIN9_FUNCTION			IMU_PIN9_FUNCTION_INT2







// #################################### Accelerometer Options

/*
 * 		IMU_SETUP_ACCEL_MODE			Accelerometer Mode Selection
 *
 * 		Disabled						IMU_ACCEL_MODE_OFF
 * 		Low Power Mode (LP)				IMU_ACCEL_MODE_LP
 * 		Low Noise Mode (LN)				IMU_ACCEL_MODE_LN
 */
#define IMU_SETUP_ACCEL_MODE			IMU_ACCEL_MODE_LN

/*
 * 		IMU_SETUP_ACCEL_FS				Accelerometer Full Scale Selection
 *
 * 		+-16g							IMU_ACCEL_UI_FS_SEL_16G
 * 		+-8g							IMU_ACCEL_UI_FS_SEL_8G
 * 		+-4g							IMU_ACCEL_UI_FS_SEL_4G
 * 		+-2g							IMU_ACCEL_UI_FS_SEL_2G
 */
#define IMU_SETUP_ACCEL_FS				IMU_ACCEL_UI_FS_SEL_16G

/*
 * 		IMU_SETUP_ACCEL_ODR				Accelerometer Output Data Rate Selection
 *
 * 		32kHz (LN Mode Only)			IMU_ACCEL_ODR_32KHZ
 * 		16kHz (LN Mode Only)			IMU_ACCEL_ODR_16KHZ
 * 		8kHz (LN Mode Only)				IMU_ACCEL_ODR_8KHZ
 * 		4kHz (LN Mode Only)				IMU_ACCEL_ODR_4KHZ
 * 		2kHz (LN Mode Only)				IMU_ACCEL_ODR_2KHZ
 * 		1kHz (LN Mode Only)				IMU_ACCEL_ODR_1KHZ
 * 		200Hz (LP/LN Modes)				IMU_ACCEL_ODR_200HZ
 * 		100Hz (LP/LN Modes)				IMU_ACCEL_ODR_100HZ
 * 		50Hz (LP/LN Modes)				IMU_ACCEL_ODR_50HZ
 * 		25Hz (LP/LN Modes)				IMU_ACCEL_ODR_25HZ
 * 		12.5Hz (LP/LN Modes)			IMU_ACCEL_ODR_12_5HZ
 * 		6.25Hz (LP Mode Only)			IMU_ACCEL_ODR_6_25HZ
 * 		3.125Hz (LP Mode Only)			IMU_ACCEL_ODR_3_125HZ
 * 		1.5625Hz (LP Mode Only)			IMU_ACCEL_ODR_1_5625HZ
 * 		500Hz (LP/LN Modes)				IMU_ACCEL_ODR_500HZ
 */
#define IMU_SETUP_ACCEL_ODR				IMU_ACCEL_ODR_1KHZ

/*
 * 		IMU_SETUP_ACCEL_LPCLK			Accelerometer Clock Selection for Low Power Mode
 *
 * 		Wake Up Clock					IMU_ACCEL_LP_CLK_SEL_WKUP
 * 		RTC Clock						IMU_ACCEL_LP_CLK_SEL_RTC
 */
#define IMU_SETUP_ACCEL_LPCLK			IMU_ACCEL_LP_CLK_SEL_WKUP







// #################################### Gyroscope Options

/*
 * 		IMU_SETUP_GYRO_MODE				Gyroscope Mode Selection
 *
 * 		Disabled						IMU_GYRO_MODE_OFF
 * 		Standby Mode					IMU_GYRO_MODE_STANDBY
 * 		Low Noise Mode (LN)				IMU_GYRO_MODE_LN
 */
#define IMU_SETUP_GYRO_MODE				IMU_GYRO_MODE_LN

/*
 * 		IMU_SETUP_GYRO_FS				Gyroscope Full Scale Selection
 *
 * 		+-2000dps						IMU_GYRO_UI_FS_SEL_2000DPS
 * 		+-1000dps						IMU_GYRO_UI_FS_SEL_1000DPS
 * 		+-500dps						IMU_GYRO_UI_FS_SEL_500DPS
 * 		+-250dps						IMU_GYRO_UI_FS_SEL_250DPS
 * 		+-125dps						IMU_GYRO_UI_FS_SEL_125DPS
 * 		+-62.5dps						IMU_GYRO_UI_FS_SEL_62_5DPS
 * 		+-31.25dps						IMU_GYRO_UI_FS_SEL_31_25DPS
 * 		+-15.625dps						IMU_GYRO_UI_FS_SEL_15_625DPS
 */
#define IMU_SETUP_GYRO_FS				IMU_GYRO_UI_FS_SEL_2000DPS

/*
 * 		IMU_SETUP_GYRO_ODR				Gyroscope Output Data Rate Selection
 *
 * 		32kHz							IMU_GYRO_ODR_32KHZ
 * 		16kHz							IMU_GYRO_ODR_16KHZ
 * 		8kHz							IMU_GYRO_ODR_8KHZ
 * 		4kHz							IMU_GYRO_ODR_4KHZ
 * 		2kHz							IMU_GYRO_ODR_2KHZ
 * 		1kHz							IMU_GYRO_ODR_1KHZ
 * 		200Hz							IMU_GYRO_ODR_200HZ
 * 		100Hz							IMU_GYRO_ODR_100HZ
 * 		50Hz							IMU_GYRO_ODR_50HZ
 * 		25Hz							IMU_GYRO_ODR_25HZ
 * 		12.5Hz							IMU_GYRO_ODR_12_5HZ
 * 		500Hz							IMU_GYRO_ODR_500HZ
 */
#define IMU_SETUP_GYRO_ODR				IMU_GYRO_ODR_1KHZ







// #################################### Temperature Sensor Options

/*
 * 		IMU_SETUP_TEMP_MODE				Temperature Sensor Mode
 *
 * 		Enabled							IMU_TEMP_EN
 * 		Disabled						IMU_TEMP_DIS
 */
#define IMU_SETUP_TEMP_MODE				IMU_TEMP_EN







// #################################### FIFO Options

/*
 * 		IMU_SETUP_FIFO_MODE				FIFO Mode Selection
 *
 * 		Bypass FIFO						IMU_FIFO_MODE_BYPASS
 * 		Stream-to-FIFO					IMU_FIFO_MODE_STREAM
 * 		STOP-on-FULL					IMU_FIFO_MODE_STOPONFULL
 */
#define IMU_SETUP_FIFO_MODE				IMU_FIFO_MODE_STREAM

/*
 * 		IMU_SETUP_FIFO_PACKET			Components of FIFO packet
 * 										Use bitwise OR ( | ) for multiple selection
 * 										See datasheet chapter 6 for details
 *
 * 		6-byte Accelerometer Data		IMU_FIFO_ACCEL_EN
 *		6-byte Gyroscope Data			IMU_FIFO_GYRO_EN
 *		FSYNC time and flag on TMST		IMU_FIFO_TMST_FSYNC_EN
 *		3-byte 20-bit Extension			IMU_HIRES_EN
 */
#define IMU_SETUP_FIFO_PACKET			(IMU_FIFO_ACCEL_EN | IMU_FIFO_GYRO_EN | IMU_FIFO_TMST_FSYNC_EN)

/*
 * 		IMU_SETUP_FIFO_HOLD_LAST_DATA	Determines what to do with invalid data samples
 * 										See datasheet chapter 14.34 for details
 *
 * 		Enabled							IMU_FIFO_HOLD_LAST_DATA_EN
 * 		Disabled						IMU_FIFO_HOLD_LAST_DATA_DIS
 */
#define IMU_SETUP_FIFO_HOLD_LAST_DATA	IMU_FIFO_HOLD_LAST_DATA_DIS

/*
 * 		IMU_SETUP_FIFO_WM_GT_TH			If enabled, fires an interrupt if FIFO Count >= WM
 *
 * 		Enabled							IMU_FIFO_WM_GT_TH_EN
 * 		Disabled						IMU_FIFO_WM_GT_TH_DIS
 */
#define IMU_SETUP_FIFO_WM_GT_TH			IMU_FIFO_WM_GT_TH_DIS

/*
 * 		IMU_SETUP_FIFO_WM				WM Value
 * 										Note: IMU_Init automatically converts to records if FIFO Count counts records
 * 										See advanced options at the bottom of this file (IMU_SETUP_FIFO_COUNT_REC)
 *
 * 		x = 1 to 4095					x
 */
#define IMU_SETUP_FIFO_WM				120







// #################################### Timestamp and FSYNC Options

/*
 * 		IMU_SETUP_TMST_EN				Timestamp Register
 *
 * 		Enabled							IMU_TMST_EN
 * 		Disabled						IMU_TMST_DIS
 */
#define IMU_SETUP_TMST_EN				IMU_TMST_EN

/*
 * 		IMU_SETUP_TMST_FSYNC_EN			Timestamp Register FSYNC
 *
 * 		Enabled							IMU_TMST_FSYNC_EN
 * 		Disabled						IMU_TMST_FSYNC_DIS
 */
#define IMU_SETUP_TMST_FSYNC_EN			IMU_TMST_FSYNC_EN

/*
 * 		IMU_SETUP_TMST_RES				Timestamp Resolution
 *
 * 		1us (microsecond)				IMU_TMST_RES_1US
 * 		16us/1 RTC period if enabled	IMU_TMST_RES_16US_OR_RTC
 */
#define IMU_SETUP_TMST_RES				IMU_TMST_RES_1US

/*
 * 		IMU_SETUP_TMST_DELTA_EN			If enabled, timestamp contains the time since last ODR
 *
 * 		Enabled							IMU_TMST_DELTA_EN
 * 		Disabled						IMU_TMST_DELTA_DIS
 */
#define IMU_SETUP_TMST_DELTA_EN			IMU_TMST_DELTA_EN

/*
 * 		IMU_SETUP_TMST_TO_REGS_EN		If enabled, timestamp value is routed to TMST_VALUE registers
 *
 * 		Enabled							IMU_TMST_TO_REGS_EN
 * 		Disabled						IMU_TMST_TO_REGS_DIS
 */
#define IMU_SETUP_TMST_TO_REGS_EN		IMU_TMST_TO_REGS_EN

/*
 * 		IMU_SETUP_FSYNC_POLARITY		FSYNC edge to measure interval
 *
 * 		Rising Edge						IMU_FSYNC_POLARITY_RE
 * 		Falling Edge					IMU_FSYNC_POLARITY_FE
 */
#define IMU_SETUP_FSYNC_POLARITY		IMU_FSYNC_POLARITY_RE

/*
 * 		IMU_SETUP_FSYNC_UI_FLAG_CLEAR_SEL	FSYNC Flag Clear Options
 *
 * 		Clear on sensor register update		IMU_FSYNC_UI_FLAG_CLEAR_SEL_UPD
 * 		Clear on FSYNC tagged register		IMU_FSYNC_UI_FLAG_CLEAR_SEL_R
 */
#define IMU_SETUP_FSYNC_UI_FLAG_CLEAR_SEL	IMU_FSYNC_UI_FLAG_CLEAR_SEL_UPD

/*
 * 		IMU_SETUP_FSYNC_UI_SEL			FSYNC Tagged Register (LSB is tagged)
 *
 *		None							IMU_FSYNC_UI_SEL_NONE
 *		TEMP_OUT						IMU_FSYNC_UI_SEL_TEMP_OUT
 *		GYRO_XOUT						IMU_FSYNC_UI_SEL_GYRO_XOUT
 *		GYRO_YOUT						IMU_FSYNC_UI_SEL_GYRO_YOUT
 *		GYRO_ZOUT						IMU_FSYNC_UI_SEL_GYRO_ZOUT
 *		ACCEL_XOUT						IMU_FSYNC_UI_SEL_ACCEL_XOUT
 *		ACCEL_YOUT						IMU_FSYNC_UI_SEL_ACCEL_YOUT
 *		ACCEL_ZOUT						IMU_FSYNC_UI_SEL_ACCEL_ZOUT
 */
#define IMU_SETUP_FSYNC_UI_SEL			IMU_FSYNC_UI_SEL_TEMP_OUT







// #################################### Interrupt Options

/*
 * 		IMU_SETUP_INTx_MODE				Interrupt Mode Selection
 * 		x = 1 or 2
 *
 * 		Pulsed							IMU_INTx_MODE_PULSED (INT signal automatically turns off after a pulse)
 * 		Latched							IMU_INTx_MODE_LATCHED (INT signal is turned off by reading INT_STATUS)
 */
#define IMU_SETUP_INT1_MODE				IMU_INT1_MODE_PULSED
#define IMU_SETUP_INT2_MODE				IMU_INT2_MODE_PULSED

/*
 * 		IMU_SETUP_INTx_DRIVE_CIRCUIT	Interrupt Driving Circuit Configuration
 * 		x = 1 or 2
 *
 * 		Open Drain						IMU_INTx_DRIVE_CIRCUIT_OD
 * 		Push-Pull						IMU_INTx_DRIVE_CIRCUIT_PP
 */
#define IMU_SETUP_INT1_DRIVE_CIRCUIT	IMU_INT1_DRIVE_CIRCUIT_PP
#define IMU_SETUP_INT2_DRIVE_CIRCUIT	IMU_INT2_DRIVE_CIRCUIT_PP

/*
 * 		IMU_SETUP_INTx_POLARITY			Interrupt Signal Polarity
 * 		x = 1 or 2
 *
 * 		Active High						IMU_INTx_POLARITY_HIGH
 * 		Active Low						IMU_INTx_POLARITY_LOW
 */
#define IMU_SETUP_INT1_POLARITY			IMU_INT1_POLARITY_HIGH
#define IMU_SETUP_INT2_POLARITY			IMU_INT2_POLARITY_HIGH

/*
 * 		IMU_SETUP_INTx_SRC				Interrupt Sources for Pin x
 * 		x = 1 or 2						To have multiple sources, put bitwise or ( | ) between the sources
 * 										Read INT_STATUS to check for source
 *
 * 		No Interrupts					0
 * 		AGC Ready						IMU_UI_AGC_RDY_INTx_EN
 * 		FIFO Full						IMU_FIFO_FULL_INTx_EN
 * 		FIFO Threshold					IMU_FIFO_THS_INTx_EN
 * 		Data Ready						IMU_UI_DRDY_INTx_EN
 *		Reset Done						IMU_RESET_DONE_INTx_EN
 *		PLL Ready						IMU_PLL_RDY_INTx_EN
 *		UI FSYNC Interrupt				IMU_UI_FSYNC_INTx_EN
 */
#define IMU_SETUP_INT1_SRC				0
#define IMU_SETUP_INT2_SRC				0

/*
 * 		IMU_SETUP_INT_CONFIG0			INT_CONFIG0 Register
 * 										See datasheet chapter 14.49 for options
 * 										As default (0), interrupts are cleared on INT_STATUS read
 */
#define IMU_SETUP_INT_CONFIG0			0

/*
 * 		IMU_SETUP_INT_CONFIG1			INT_CONFIG1 Register
 * 										See datasheet chapter 14.50 for options
 * 										Use bitwise OR ( | ) for multiple selection
 *
 * 		Async Reset						IMU_INT_ASYNC_RESET
 * 		TDeassert Disable				IMU_INT_TDEASSERT_DISABLE
 * 		Pulse Duration 100us			IMU_INT_TPULSE_DURATION_100US
 * 		Pulse Duration 8us				IMU_INT_TPULSE_DURATION_8US
 */
#define IMU_SETUP_INT_CONFIG1			IMU_INT_TPULSE_DURATION_100US







// #################################### Anti-Alias Filter Options

/*
 * 		IMU_SETUP_ACCEL_AAF_EN			Accelerometer Anti-Alias Filter
 *
 * 		Enabled							IMU_ACCEL_AAF_EN
 * 		Disabled						IMU_ACCEL_AAF_DIS
 */
#define IMU_SETUP_ACCEL_AAF_EN			IMU_ACCEL_AAF_EN

/*
 * 		IMU_SETUP_ACCEL_AAF_DELT		Delta Value for Accelerometer Anti-Alias Filter
 * 										See datasheet chapter 5.3 for AAF Bandwidth Options
 * 										DELTSQR and BANDWIDTH are automatically selected for desired bandwidth (See IMU_Init)
 *
 * 		42Hz  (16ms  )					IMU_ACCEL_AAF_DELT_42HZ
 * 		84Hz  (8ms   )					IMU_ACCEL_AAF_DELT_84HZ
 * 		126Hz (5.3ms )					IMU_ACCEL_AAF_DELT_126HZ
 * 		170Hz (4ms   )					IMU_ACCEL_AAF_DELT_170HZ
 * 		213Hz (3.2ms )					IMU_ACCEL_AAF_DELT_213HZ
 * 		258Hz (2.6ms )					IMU_ACCEL_AAF_DELT_258HZ
 * 		303Hz (2.2ms )					IMU_ACCEL_AAF_DELT_303HZ
 * 		348Hz (1.9ms )					IMU_ACCEL_AAF_DELT_348HZ
 * 		394Hz (1.7ms )					IMU_ACCEL_AAF_DELT_394HZ
 * 		441Hz (1.5ms )					IMU_ACCEL_AAF_DELT_441HZ
 * 		488Hz (1.3ms )					IMU_ACCEL_AAF_DELT_488HZ
 * 		536Hz (1.2ms )					IMU_ACCEL_AAF_DELT_536HZ
 * 		585Hz (1.1ms )					IMU_ACCEL_AAF_DELT_585HZ
 * 		634Hz (1ms   )					IMU_ACCEL_AAF_DELT_634HZ
 * 		684Hz (0.9ms )					IMU_ACCEL_AAF_DELT_684HZ
 * 		734Hz (0.9ms )					IMU_ACCEL_AAF_DELT_734HZ
 * 		785Hz (0.8ms )					IMU_ACCEL_AAF_DELT_785HZ
 * 		837Hz (0.8ms )					IMU_ACCEL_AAF_DELT_837HZ
 * 		890Hz (0.7ms )					IMU_ACCEL_AAF_DELT_890HZ
 * 		943Hz (0.7ms )					IMU_ACCEL_AAF_DELT_943HZ
 * 		997Hz (0.6ms )					IMU_ACCEL_AAF_DELT_997HZ
 * 		1051Hz (0.6ms)					IMU_ACCEL_AAF_DELT_1051HZ
 * 		1107Hz (0.6ms)					IMU_ACCEL_AAF_DELT_1107HZ
 * 		1163Hz (0.5ms)					IMU_ACCEL_AAF_DELT_1163HZ
 * 		1220Hz (0.5ms)					IMU_ACCEL_AAF_DELT_1220HZ
 * 		1277Hz (0.5ms)					IMU_ACCEL_AAF_DELT_1277HZ
 * 		1336Hz (0.5ms)					IMU_ACCEL_AAF_DELT_1336HZ
 * 		1395Hz (0.5ms)					IMU_ACCEL_AAF_DELT_1395HZ
 * 		1454Hz (0.4ms)					IMU_ACCEL_AAF_DELT_1454HZ
 * 		1515Hz (0.4ms)					IMU_ACCEL_AAF_DELT_1515HZ
 * 		1577Hz (0.4ms)					IMU_ACCEL_AAF_DELT_1577HZ
 * 		1639Hz (0.4ms)					IMU_ACCEL_AAF_DELT_1639HZ
 * 		1702Hz (0.4ms)					IMU_ACCEL_AAF_DELT_1702HZ
 * 		1766Hz (0.4ms)					IMU_ACCEL_AAF_DELT_1766HZ
 * 		1830Hz (0.3ms)					IMU_ACCEL_AAF_DELT_1830HZ
 * 		1896Hz (0.3ms)					IMU_ACCEL_AAF_DELT_1896HZ
 * 		1962Hz (0.3ms)					IMU_ACCEL_AAF_DELT_1962HZ
 * 		2029Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2029HZ
 * 		2097Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2097HZ
 * 		2166Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2166HZ
 * 		2235Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2235HZ
 * 		2306Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2306HZ
 * 		2377Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2377HZ
 * 		2449Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2449HZ
 * 		2522Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2522HZ
 * 		2596Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2596HZ
 * 		2671Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2671HZ
 * 		2746Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2746HZ
 * 		2823Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2823HZ
 * 		2900Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2900HZ
 * 		2978Hz (0.3ms)					IMU_ACCEL_AAF_DELT_2978HZ
 * 		3057Hz (0.3ms)					IMU_ACCEL_AAF_DELT_3057HZ
 * 		3137Hz (0.3ms)					IMU_ACCEL_AAF_DELT_3137HZ
 * 		3217Hz (0.3ms)					IMU_ACCEL_AAF_DELT_3217HZ
 * 		3299Hz (0.2ms)					IMU_ACCEL_AAF_DELT_3299HZ
 * 		3381Hz (0.2ms)					IMU_ACCEL_AAF_DELT_3381HZ
 * 		3464Hz (0.2ms)					IMU_ACCEL_AAF_DELT_3464HZ
 * 		3548Hz (0.2ms)					IMU_ACCEL_AAF_DELT_3548HZ
 * 		3633Hz (0.2ms)					IMU_ACCEL_AAF_DELT_3633HZ
 * 		3718Hz (0.2ms)					IMU_ACCEL_AAF_DELT_3718HZ
 * 		3805Hz (0.2ms)					IMU_ACCEL_AAF_DELT_3805HZ
 * 		3892Hz (0.2ms)					IMU_ACCEL_AAF_DELT_3892HZ
 * 		3979Hz (0.2ms)					IMU_ACCEL_AAF_DELT_3979HZ
 * 		
 * 		Desired AAF Bandwidth			IMU_ACCEL_AAF_DELT_xxxxHZ
 */
#define IMU_SETUP_ACCEL_AAF_DELT		IMU_ACCEL_AAF_DELT_1051HZ

/*
 * 		IMU_SETUP_GYRO_AAF_EN			Gyroscope Anti-Alias Filter
 *
 * 		Enabled							IMU_GYRO_AAF_EN
 * 		Disabled						IMU_GYRO_AAF_DIS
 */
#define IMU_SETUP_GYRO_AAF_EN			IMU_GYRO_AAF_EN

/*
 * 		IMU_SETUP_GYRO_AAF_DELT			Delta Value for Gyroscope Anti-Alias Filter
 * 										See datasheet chapter 5.3 for AAF Bandwidth Options
 * 										DELTSQR and BANDWIDTH are automatically selected for desired bandwidth (See IMU_Init)
 *
 * 		Desired AAF Bandwidth			IMU_GYRO_AAF_DELT_xxxxHZ
 */
#define IMU_SETUP_GYRO_AAF_DELT			IMU_GYRO_AAF_DELT_1051HZ







// #################################### UI Filter Options

/*
 * 		IMU_SETUP_ACCEL_UI_FILT_ORD		Accelerometer UI Filter Order
 *
 *		1st Order						IMU_ACCEL_UI_FILT_ORD_1ST
 *		2nd Order						IMU_ACCEL_UI_FILT_ORD_2ND
 *		3rd Order						IMU_ACCEL_UI_FILT_ORD_3RD
 */
#define IMU_SETUP_ACCEL_UI_FILT_ORD		IMU_ACCEL_UI_FILT_ORD_1ST

/*
 * 		IMU_SETUP_ACCEL_UI_FILT_BW		Accelerometer UI Filter (LPF) Bandwidth
 *
 *		ODR/2 (LN)						IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV2
 *		max(400Hz, ODR)/4 (LN)			IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV4
 *		max(400Hz, ODR)/5 (LN)			IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV5
 *		max(400Hz, ODR)/8 (LN)			IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV8
 *		max(400Hz, ODR)/10 (LN)			IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV10
 *		max(400Hz, ODR)/16 (LN)			IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV16
 *		max(400Hz, ODR)/20 (LN)			IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV20
 *		max(400Hz, ODR)/40 (LN)			IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV40
 *		See datasheet chapter 14.40 for low latency options
 *		Low Latency 14 (LN)				IMU_ACCEL_UI_FILT_BW_LN_DEC2_ODR
 *		Low Latency 15 (LN)				IMU_ACCEL_UI_FILT_BW_LN_DEC2_ODR8
 *		1xAVG (LP)						IMU_ACCEL_UI_FILT_BW_LP_AVG
 *		16xAVG (LP)						IMU_ACCEL_UI_FILT_BW_LP_AVG16
 */
#define IMU_SETUP_ACCEL_UI_FILT_BW		IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV4

/*
 * 		IMU_SETUP_GYRO_UI_FILT_ORD		Gyroscope UI Filter Order
 *
 *		1st Order						IMU_GYRO_UI_FILT_ORD_1ST
 *		2nd Order						IMU_GYRO_UI_FILT_ORD_2ND
 *		3rd Order						IMU_GYRO_UI_FILT_ORD_3RD
 */
#define IMU_SETUP_GYRO_UI_FILT_ORD		IMU_GYRO_UI_FILT_ORD_1ST

/*
 * 		IMU_SETUP_GYRO_UI_FILT_BW		Gyroscope UI Filter (LPF) Bandwidth
 *
 *		ODR/2							IMU_GYRO_UI_FILT_BW_ODR_DIV2
 *		max(400Hz, ODR)/4				IMU_GYRO_UI_FILT_BW_ODR_DIV4
 *		max(400Hz, ODR)/5				IMU_GYRO_UI_FILT_BW_ODR_DIV5
 *		max(400Hz, ODR)/8				IMU_GYRO_UI_FILT_BW_ODR_DIV8
 *		max(400Hz, ODR)/10				IMU_GYRO_UI_FILT_BW_ODR_DIV10
 *		max(400Hz, ODR)/16				IMU_GYRO_UI_FILT_BW_ODR_DIV16
 *		max(400Hz, ODR)/20				IMU_GYRO_UI_FILT_BW_ODR_DIV20
 *		max(400Hz, ODR)/40				IMU_GYRO_UI_FILT_BW_ODR_DIV40
 *
 *		See datasheet chapter 14.40 for low latency options
 *		Low Latency 14					IMU_GYRO_UI_FILT_BW_DEC2_ODR
 *		Low Latency 15					IMU_GYRO_UI_FILT_BW_DEC2_ODR8
 */
#define IMU_SETUP_GYRO_UI_FILT_BW		IMU_GYRO_UI_FILT_BW_ODR_DIV4

/*
 * 		IMU_SETUP_TEMP_FILT_BW			Temperature Sensor Filter (DLPF) Bandwidth
 *
 * 		4000Hz, 0.125ms Latency			IMU_TEMP_FILT_BW_4KHZ_0_125MS
 * 		170Hz, 1ms Latency				IMU_TEMP_FILT_BW_170HZ_1MS
 * 		82Hz, 2ms Latency				IMU_TEMP_FILT_BW_82HZ_2MS
 * 		40Hz, 4ms Latency				IMU_TEMP_FILT_BW_40HZ_4MS
 * 		20Hz, 8ms Latency				IMU_TEMP_FILT_BW_20HZ_8MS
 * 		10Hz, 16ms Latency				IMU_TEMP_FILT_BW_10HZ_16MS
 * 		5Hz, 32ms Latency				IMU_TEMP_FILT_BW_5HZ_32MS
 */
#define IMU_SETUP_TEMP_FILT_BW			IMU_TEMP_FILT_BW_4KHZ_0_125MS







// #################################### Notch Filter Options

/*
 * 		IMU_SETUP_GYRO_NF_EN			Notch Filter
 *
 * 		Enabled							IMU_GYRO_NF_EN
 * 		Disabled						IMU_GYRO_NF_DIS
 */
#define IMU_SETUP_GYRO_NF_EN			IMU_GYRO_NF_EN

/*
 * 		IMU_SETUP_GYRO_NF_BW			Notch Filter Bandwidth (All 3 axes)
 *
 * 		1449Hz							IMU_GYRO_NF_BW_SEL_1449HZ
 * 		680Hz							IMU_GYRO_NF_BW_SEL_680HZ
 * 		329Hz							IMU_GYRO_NF_BW_SEL_329HZ
 * 		162Hz							IMU_GYRO_NF_BW_SEL_162HZ
 * 		80Hz							IMU_GYRO_NF_BW_SEL_80HZ
 * 		40Hz							IMU_GYRO_NF_BW_SEL_40HZ
 * 		20Hz							IMU_GYRO_NF_BW_SEL_20HZ
 * 		10Hz							IMU_GYRO_NF_BW_SEL_10HZ
 */
#define IMU_SETUP_GYRO_NF_BW			IMU_GYRO_NF_BW_SEL_1449HZ

/*
 * 		IMU_SETUP_GYRO_NF_CLKDIV		Clock Divider for Notch Filter (19.2 MHz / (CLKDIV * 10))
 * 										See datasheet chapter 5.2 for details
 *
 *		x = 1 to 127					IMU_CLKDIV_1_TO_127(x)
 */
#define IMU_SETUP_GYRO_NF_CLKDIV		IMU_CLKDIV_1_TO_127(1)

/*
 * 		IMU_SETUP_GYRO_NF_xyz_FREQ		Desired Notch Filter Frequency
 * 										Minimum is 1000Hz, Maximum is 3000Hz
 *
 *		x = 1000 to 3000				x
 */
#define IMU_SETUP_GYRO_NF_X_FREQ		1000
#define IMU_SETUP_GYRO_NF_Y_FREQ		1000
#define IMU_SETUP_GYRO_NF_Z_FREQ		1000







/*
 * 		IMU_SETUP_SENSOR_CONFIG0		SENSOR_CONFIG0 Register
 * 										Enable/Disable specific axes
 * 										Use bitwise OR ( | ) for multiple selection
 * 										0 = All Enabled
 *
 * 		Disable Accel X					IMU_XA_DISABLE
 * 		Disable Accel Y					IMU_YA_DISABLE
 * 		Disable Accel Z					IMU_ZA_DISABLE
 * 		Disable Gyro X					IMU_XG_DISABLE
 * 		Disable Gyro Y					IMU_YG_DISABLE
 * 		Disable Gyro Z					IMU_ZG_DISABLE
 */
#define IMU_SETUP_SENSOR_CONFIG0		0

// #################################### APEX Options

/*
 * 		TO BE IMPLEMENTED
 * 		APEX FEATURES ARE NOT FOR AUVS, SKIPPED FOR NOW
 */







// #################################### Advanced Options

/*
 * 		IMU_SETUP_RTC_MODE				Enforce RTC clock input
 *
 * 		Disabled						IMU_RTC_MODE_DIS
 * 		Enabled							IMU_RTC_MODE_EN
 */
#define IMU_SETUP_RTC_MODE				IMU_RTC_MODE_DIS

/*
 * 		IMU_SETUP_SENSOR_ENDIAN			Sensor Data Endian Format Selection
 *
 * 		Little Endian Format			IMU_SENSOR_DATA_ENDIAN_LITTLE
 * 		Big Endian Format				IMU_SENSOR_DATA_ENDIAN_BIG
 */
#define IMU_SETUP_SENSOR_ENDIAN			IMU_SENSOR_DATA_ENDIAN_BIG

/*
 * 		IMU_SETUP_SPI_SLEW				SPI Slew Rate
 *
 * 		20 to 60 ns						IMU_SPI_SLEW_RATE_20TO60NS
 * 		12 to 36 ns						IMU_SPI_SLEW_RATE_12TO36NS
 * 		6 to 18 ns						IMU_SPI_SLEW_RATE_6TO18NS
 * 		4 to 12 ns						IMU_SPI_SLEW_RATE_4TO12NS
 * 		2 to 6 ns						IMU_SPI_SLEW_RATE_2TO6NS
 * 		Up to 2 ns						IMU_SPI_SLEW_RATE_0TO2NS
 */
#define IMU_SETUP_SPI_SLEW				IMU_SPI_SLEW_RATE_0TO2NS

/*
 * 		IMU_SETUP_I2C_SLEW				I2C Slew Rate
 *
 * 		20 to 60 ns						IMU_I2C_SLEW_RATE_20TO60NS
 * 		12 to 36 ns						IMU_I2C_SLEW_RATE_12TO36NS
 * 		6 to 18 ns						IMU_I2C_SLEW_RATE_6TO18NS
 * 		4 to 12 ns						IMU_I2C_SLEW_RATE_4TO12NS
 * 		2 to 6 ns						IMU_I2C_SLEW_RATE_2TO6NS
 * 		Up to 2 ns						IMU_I2C_SLEW_RATE_0TO2NS
 */
#define IMU_SETUP_I2C_SLEW				IMU_I2C_SLEW_RATE_0TO2NS

/*
 * 		IMU_SETUP_FIFO_COUNT_ENDIAN		FIFO Count Endian Format Selection
 *
 * 		Little Endian Format			IMU_FIFO_COUNT_ENDIAN_LITTLE
 * 		Big Endian Format				IMU_FIFO_COUNT_ENDIAN_BIG
 */
#define IMU_SETUP_FIFO_COUNT_ENDIAN		IMU_FIFO_COUNT_ENDIAN_BIG

/*
 * 		IMU_SETUP_FIFO_COUNT_REC		FIFO Count Report Format
 *
 * 		Bytes							IMU_FIFO_COUNT_REC_BYTES
 * 		Records (Packet Count)			IMU_FIFO_COUNT_REC_RECORDS
 */
#define IMU_SETUP_FIFO_COUNT_REC		IMU_FIFO_COUNT_REC_BYTES

/*
 * 		IMU_SETUP_FIFO_PARTIAL_RD		FIFO Partial Read
 *
 * 		Disabled						IMU_FIFO_RESUME_PARTIAL_RD_DIS
 * 		Enabled (Not Recommended)		IMU_FIFO_RESUME_PARTIAL_RD_EN
 */
#define IMU_SETUP_FIFO_PARTIAL_RD		IMU_FIFO_RESUME_PARTIAL_RD_DIS



#endif /* IMU_CONFIG_H_ */
