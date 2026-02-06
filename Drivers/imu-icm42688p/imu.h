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

/*	#############################################
 * 	         			imu.h
 * 	      Header File for ICM-42688-P Driver
 * 	      	See "imu_config.h" for options
 *	############################################# */

#ifndef INC_IMU_H_
#define INC_IMU_H_

#include <stm32g4xx_hal.h>

#define IMU_I2C_ADDRESS_AD0_LOW			0xD0
#define IMU_I2C_ADDRESS_AD0_HIGH		0xD1

typedef enum{
	IMU_OK = 0,
	IMU_ERROR = 1,
	IMU_BUSY = 2,
	IMU_TIMEOUT = 3,
	IMU_INVALID_CONFIG = 4
}IMU_Status_t;

/*	#############################################
 * 	         Register Address Definitions
 *	############################################# */

// #################################### USR Bank 0
#define IMU_REG_DEVICE_CONFIG			0x11
#define IMU_REG_DRIVE_CONFIG 			0x13
#define IMU_REG_INT_CONFIG 				0x14
#define IMU_REG_FIFO_CONFIG 			0x16
#define IMU_REG_TEMP_DATA1 				0x1D
#define IMU_REG_TEMP_DATA0 				0x1E
#define IMU_REG_ACCEL_DATA_X1 			0x1F
#define IMU_REG_ACCEL_DATA_X0 			0x20
#define IMU_REG_ACCEL_DATA_Y1 			0x21
#define IMU_REG_ACCEL_DATA_Y0 			0x22
#define IMU_REG_ACCEL_DATA_Z1 			0x23
#define IMU_REG_ACCEL_DATA_Z0 			0x24
#define IMU_REG_GYRO_DATA_X1 			0x25
#define IMU_REG_GYRO_DATA_X0 			0x26
#define IMU_REG_GYRO_DATA_Y1 			0x27
#define IMU_REG_GYRO_DATA_Y0 			0x28
#define IMU_REG_GYRO_DATA_Z1 			0x29
#define IMU_REG_GYRO_DATA_Z0 			0x2A
#define IMU_REG_TMST_FSYNCH 			0x2B
#define IMU_REG_TMST_FSYNCL 			0x2C
#define IMU_REG_INT_STATUS				0x2D
#define IMU_REG_FIFO_COUNTH				0x2E
#define IMU_REG_FIFO_COUNTL				0x2F
#define IMU_REG_FIFO_DATA				0x30
#define IMU_REG_APEX_DATA0				0x31
#define IMU_REG_APEX_DATA1				0x32
#define IMU_REG_APEX_DATA2				0x33
#define IMU_REG_APEX_DATA3				0x34
#define IMU_REG_APEX_DATA4				0x35
#define IMU_REG_APEX_DATA5				0x36
#define IMU_REG_INT_STATUS2				0x37
#define IMU_REG_INT_STATUS3				0x38
#define IMU_REG_SIGNAL_PATH_RESET		0x4B
#define IMU_REG_INTF_CONFIG0			0x4C
#define IMU_REG_INTF_CONFIG1			0x4D
#define IMU_REG_PWR_MGMT0				0x4E
#define IMU_REG_GYRO_CONFIG0			0x4F
#define IMU_REG_ACCEL_CONFIG0			0x50
#define IMU_REG_GYRO_CONFIG1			0x51
#define IMU_REG_GYRO_ACCEL_CONFIG0		0x52
#define IMU_REG_ACCEL_CONFIG1			0x53
#define IMU_REG_TMST_CONFIG				0x54
#define IMU_REG_APEX_CONFIG0			0x56
#define IMU_REG_SMD_CONFIG				0x57
#define IMU_REG_FIFO_CONFIG1			0x5F
#define IMU_REG_FIFO_CONFIG2			0x60
#define IMU_REG_FIFO_CONFIG3			0x61
#define IMU_REG_FSYNC_CONFIG			0x62
#define IMU_REG_INT_CONFIG0				0x63
#define IMU_REG_INT_CONFIG1				0x64
#define IMU_REG_INT_SOURCE0				0x65
#define IMU_REG_INT_SOURCE1				0x66
#define IMU_REG_INT_SOURCE3				0x68
#define IMU_REG_INT_SOURCE4				0x69
#define IMU_REG_FIFO_LOST_PKT0			0x6C
#define IMU_REG_FIFO_LOST_PKT1			0x6D
#define IMU_REG_SELF_TEST_CONFIG		0x70
#define IMU_REG_WHO_AM_I				0x75
#define IMU_REG_REG_BANK_SEL			0x76

// #################################### USR Bank 1
#define IMU_REG_SENSOR_CONFIG0			0x03
#define IMU_REG_GYRO_CONFIG_STATIC2		0x0B
#define IMU_REG_GYRO_CONFIG_STATIC3		0x0C
#define IMU_REG_GYRO_CONFIG_STATIC4		0x0D
#define IMU_REG_GYRO_CONFIG_STATIC5		0x0E
#define IMU_REG_GYRO_CONFIG_STATIC6		0x0F
#define IMU_REG_GYRO_CONFIG_STATIC7		0x10
#define IMU_REG_GYRO_CONFIG_STATIC8		0x11
#define IMU_REG_GYRO_CONFIG_STATIC9		0x12
#define IMU_REG_GYRO_CONFIG_STATIC10	0x13
#define IMU_REG_XG_ST_DATA 				0x5F
#define IMU_REG_YG_ST_DATA 				0x60
#define IMU_REG_ZG_ST_DATA 				0x61
#define IMU_REG_TMSTVAL0 				0x62
#define IMU_REG_TMSTVAL1 				0x63
#define IMU_REG_TMSTVAL2 				0x64
#define IMU_REG_INTF_CONFIG4			0x7A
#define IMU_REG_INTF_CONFIG5			0x7B
#define IMU_REG_INTF_CONFIG6			0x7C

// #################################### USR Bank 2
#define IMU_REG_ACCEL_CONFIG_STATIC2	0x03
#define IMU_REG_ACCEL_CONFIG_STATIC3	0x04
#define IMU_REG_ACCEL_CONFIG_STATIC4	0x05
#define IMU_REG_XA_ST_DATA				0x3B
#define IMU_REG_YA_ST_DATA 				0x3C
#define IMU_REG_ZA_ST_DATA 				0x3D

// #################################### USR Bank 3
#define IMU_REG_CLKDIV 					0x2A

// #################################### USR Bank 4
#define IMU_REG_APEX_CONFIG1			0x40
#define IMU_REG_APEX_CONFIG2			0x41
#define IMU_REG_APEX_CONFIG3			0x42
#define IMU_REG_APEX_CONFIG4			0x43
#define IMU_REG_APEX_CONFIG5			0x44
#define IMU_REG_APEX_CONFIG6			0x45
#define IMU_REG_APEX_CONFIG7			0x46
#define IMU_REG_APEX_CONFIG8			0x47
#define IMU_REG_APEX_CONFIG9			0x48
#define IMU_REG_ACCEL_WOM_X_THR			0x4A
#define IMU_REG_ACCEL_WOM_Y_THR			0x4B
#define IMU_REG_ACCEL_WOM_Z_THR			0x4C
#define IMU_REG_INT_SOURCE6 			0x4D
#define IMU_REG_INT_SOURCE7 			0x4E
#define IMU_REG_INT_SOURCE8 			0x4F
#define IMU_REG_INT_SOURCE9 			0x50
#define IMU_REG_INT_SOURCE10 			0x51
#define IMU_REG_OFFSET_USER0			0x77
#define IMU_REG_OFFSET_USER1			0x78
#define IMU_REG_OFFSET_USER2			0x79
#define IMU_REG_OFFSET_USER3			0x7A
#define IMU_REG_OFFSET_USER4			0x7B
#define IMU_REG_OFFSET_USER5			0x7C
#define IMU_REG_OFFSET_USER6			0x7D
#define IMU_REG_OFFSET_USER7			0x7E
#define IMU_REG_OFFSET_USER8			0x7F

/*#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################*/







/*	#############################################
 * 	         Register Bit Field Options
 *	############################################# */

// #################################### USR Bank 0
// DEVICE_CONFIG (0x11)
#define IMU_SOFT_RESET_CONFIG_0			0
#define IMU_SOFT_RESET_CONFIG_1			1
#define IMU_SPI_MODE_0_3				0
#define IMU_SPI_MODE_1_2				(1 << 4)

// DRIVE_CONFIG (0x13)
#define IMU_SPI_SLEW_RATE_20TO60NS		0
#define IMU_SPI_SLEW_RATE_12TO36NS		1
#define IMU_SPI_SLEW_RATE_6TO18NS		2
#define IMU_SPI_SLEW_RATE_4TO12NS		3
#define IMU_SPI_SLEW_RATE_2TO6NS		4
#define IMU_SPI_SLEW_RATE_0TO2NS		5
#define IMU_I2C_SLEW_RATE_20TO60NS		0
#define IMU_I2C_SLEW_RATE_12TO36NS		(1 << 3)
#define IMU_I2C_SLEW_RATE_6TO18NS		(2 << 3)
#define IMU_I2C_SLEW_RATE_4TO12NS		(3 << 3)
#define IMU_I2C_SLEW_RATE_2TO6NS		(4 << 3)
#define IMU_I2C_SLEW_RATE_0TO2NS		(5 << 3)

// INT_CONFIG (0x14)
#define IMU_INT1_POLARITY_LOW			0
#define IMU_INT1_POLARITY_HIGH			1
#define IMU_INT1_DRIVE_CIRCUIT_OD		0
#define IMU_INT1_DRIVE_CIRCUIT_PP		(1 << 1)
#define IMU_INT1_MODE_PULSED			0
#define IMU_INT1_MODE_LATCHED			(1 << 2)
#define IMU_INT2_POLARITY_LOW			0
#define IMU_INT2_POLARITY_HIGH			(1 << 3)
#define IMU_INT2_DRIVE_CIRCUIT_OD		0
#define IMU_INT2_DRIVE_CIRCUIT_PP		(1 << 4)
#define IMU_INT2_MODE_PULSED			0
#define IMU_INT2_MODE_LATCHED			(1 << 5)

// FIFO_CONFIG (0x16)
#define IMU_FIFO_MODE_BYPASS			0
#define IMU_FIFO_MODE_STREAM			(1 << 6)
#define IMU_FIFO_MODE_STOPONFULL		(2 << 6)

// TEMP_DATA1 (0x1D)
// TEMP_DATA0 (0x1E)

// ACCEL_DATA_X1 (0x1F)
// ACCEL_DATA_X0 (0x20)
// ACCEL_DATA_Y1 (0x21)
// ACCEL_DATA_Y0 (0x22)
// ACCEL_DATA_Z1 (0x23)
// ACCEL_DATA_Z0 (0x24)

// GYRO_DATA_X1 (0x25)
// GYRO_DATA_X0 (0x26)
// GYRO_DATA_Y1 (0x27)
// GYRO_DATA_Y0 (0x28)
// GYRO_DATA_Z1 (0x29)
// GYRO_DATA_Z0 (0x2A)

// TMST_FSYNCH (0x2B)
// TMST_FSYNCL (0x2C)

// INT_STATUS (0x2D)

// FIFO_COUNTH (0x2E)
// FIFO_COUNTL (0x2F)

// FIFO_DATA (0x30)

// APEX_DATA0 (0x31)
// APEX_DATA1 (0x32)
// APEX_DATA2 (0x33)
// APEX_DATA3 (0x34)
// APEX_DATA4 (0x35)
// APEX_DATA5 (0x36)

// INT_STATUS2 (0x37)
// INT_STATUS3 (0x38)

// SIGNAL_PATH_RESET (0x4B)
#define IMU_FIFO_FLUSH					(1 << 1)
#define IMU_TMST_STROBE					(1 << 2)
#define IMU_ABORT_AND_RESET 			(1 << 3)
#define IMU_DMP_MEM_RESET_EN 			(1 << 5)
#define IMU_DMP_INIT_EN					(1 << 6)

// INTF_CONFIG0 (0x4C)
#define IMU_UI_SIFS_CFG_DI_SPI			2
#define IMU_UI_SIFS_CFG_DI_I2C			3
#define IMU_SENSOR_DATA_ENDIAN_LITTLE	0
#define IMU_SENSOR_DATA_ENDIAN_BIG		(1 << 4)
#define IMU_FIFO_COUNT_ENDIAN_LITTLE	0
#define IMU_FIFO_COUNT_ENDIAN_BIG		(1 << 5)
#define IMU_FIFO_COUNT_REC_BYTES		0
#define IMU_FIFO_COUNT_REC_RECORDS		(1 << 6)
#define IMU_FIFO_HOLD_LAST_DATA_DIS		0
#define IMU_FIFO_HOLD_LAST_DATA_EN		(1 << 7)

// INTF_CONFIG1 (0x4D)
#define IMU_CLKSEL_INTERNAL				0
#define IMU_CLKSEL_PLL					1
#define IMU_CLKSEL_DISABLE				3
#define IMU_RTC_MODE_DIS				0
#define IMU_RTC_MODE_EN					(1 << 2)
#define IMU_ACCEL_LP_CLK_SEL_WKUP		0
#define IMU_ACCEL_LP_CLK_SEL_RC			(1 << 3)

// PWR_MGMT0 (0x4E)
#define IMU_ACCEL_MODE_OFF 				1
#define IMU_ACCEL_MODE_LP				2
#define IMU_ACCEL_MODE_LN				3
#define IMU_GYRO_MODE_OFF 				0
#define IMU_GYRO_MODE_STANDBY			(1 << 2)
#define IMU_GYRO_MODE_LN				(3 << 2)
#define IMU_IDLE_DIS					0
#define IMU_IDLE_EN 					(1 << 4)
#define IMU_TEMP_EN						0
#define IMU_TEMP_DIS 					(1 << 5)

// GYRO_CONFIG0 (0x4F)
#define IMU_GYRO_ODR_32KHZ				1
#define IMU_GYRO_ODR_16KHZ				2
#define IMU_GYRO_ODR_8KHZ				3
#define IMU_GYRO_ODR_4KHZ				4
#define IMU_GYRO_ODR_2KHZ				5
#define IMU_GYRO_ODR_1KHZ				6
#define IMU_GYRO_ODR_200HZ				7
#define IMU_GYRO_ODR_100HZ				8
#define IMU_GYRO_ODR_50HZ				9
#define IMU_GYRO_ODR_25HZ				10
#define IMU_GYRO_ODR_12_5HZ				11
#define IMU_GYRO_ODR_500HZ				15
#define IMU_GYRO_UI_FS_SEL_2000DPS 		0
#define IMU_GYRO_UI_FS_SEL_1000DPS		(1 << 5)
#define IMU_GYRO_UI_FS_SEL_500DPS		(2 << 5)
#define IMU_GYRO_UI_FS_SEL_250DPS		(3 << 5)
#define IMU_GYRO_UI_FS_SEL_125DPS		(4 << 5)
#define IMU_GYRO_UI_FS_SEL_62_5DPS		(5 << 5)
#define IMU_GYRO_UI_FS_SEL_31_25DPS		(6 << 5)
#define IMU_GYRO_UI_FS_SEL_15_625DPS	(7 << 5)

// ACCEL_CONFIG0 (0x50)
#define IMU_ACCEL_ODR_32KHZ				1	// LN
#define IMU_ACCEL_ODR_16KHZ				2	// LN
#define IMU_ACCEL_ODR_8KHZ				3	// LN
#define IMU_ACCEL_ODR_4KHZ				4	// LN
#define IMU_ACCEL_ODR_2KHZ				5	// LN
#define IMU_ACCEL_ODR_1KHZ				6	// LN
#define IMU_ACCEL_ODR_200HZ				7	// LP + LN
#define IMU_ACCEL_ODR_100HZ				8	// LP + LN
#define IMU_ACCEL_ODR_50HZ				9	// LP + LN
#define IMU_ACCEL_ODR_25HZ				10	// LP + LN
#define IMU_ACCEL_ODR_12_5HZ			11	// LP + LN
#define IMU_ACCEL_ODR_6_25HZ			12	// LP
#define IMU_ACCEL_ODR_3_125HZ			13	// LP
#define IMU_ACCEL_ODR_1_5625HZ			14	// LP
#define IMU_ACCEL_ODR_500HZ				15	// LP + LN
#define IMU_ACCEL_UI_FS_SEL_16G 		0
#define IMU_ACCEL_UI_FS_SEL_8G 			(1 << 5)
#define IMU_ACCEL_UI_FS_SEL_4G 			(2 << 5)
#define IMU_ACCEL_UI_FS_SEL_2G 			(3 << 5)

// GYRO_CONFIG1 (0x51)
#define IMU_GYRO_DEC2_M2_ORD_3RD		2
#define IMU_GYRO_UI_FILT_ORD_1ST		0
#define IMU_GYRO_UI_FILT_ORD_2ND		(1 << 2)
#define IMU_GYRO_UI_FILT_ORD_3RD		(2 << 2)
#define IMU_TEMP_FILT_BW_4KHZ_0_125MS	0
#define IMU_TEMP_FILT_BW_170HZ_1MS		(1 << 5)
#define IMU_TEMP_FILT_BW_82HZ_2MS		(2 << 5)
#define IMU_TEMP_FILT_BW_40HZ_4MS		(3 << 5)
#define IMU_TEMP_FILT_BW_20HZ_8MS 		(4 << 5)
#define IMU_TEMP_FILT_BW_10HZ_16MS		(5 << 5)
#define IMU_TEMP_FILT_BW_5HZ_32MS		(6 << 5)

// GYRO_ACCEL_CONFIG0 (0x52)
#define IMU_GYRO_UI_FILT_BW_ODR_DIV2		0
#define IMU_GYRO_UI_FILT_BW_ODR_DIV4		1
#define IMU_GYRO_UI_FILT_BW_ODR_DIV5		2
#define IMU_GYRO_UI_FILT_BW_ODR_DIV8		3
#define IMU_GYRO_UI_FILT_BW_ODR_DIV10		4
#define IMU_GYRO_UI_FILT_BW_ODR_DIV16		5
#define IMU_GYRO_UI_FILT_BW_ODR_DIV20		6
#define IMU_GYRO_UI_FILT_BW_ODR_DIV40		7
#define IMU_GYRO_UI_FILT_BW_DEC2_ODR		14
#define IMU_GYRO_UI_FILT_BW_DEC2_ODR8		15
#define IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV2	0
#define IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV4	(1 << 4)
#define IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV5	(2 << 4)
#define IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV8	(3 << 4)
#define IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV10	(4 << 4)
#define IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV16	(5 << 4)
#define IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV20	(6 << 4)
#define IMU_ACCEL_UI_FILT_BW_LN_ODR_DIV40	(7 << 4)
#define IMU_ACCEL_UI_FILT_BW_LN_DEC2_ODR	(14 << 4)
#define IMU_ACCEL_UI_FILT_BW_LN_DEC2_ODR8	(15 << 4)
#define IMU_ACCEL_UI_FILT_BW_LP_AVG			(1 << 4)
#define IMU_ACCEL_UI_FILT_BW_LP_AVG16		(6 << 4)

// ACCEL_CONFIG1 (0x53)
#define IMU_ACCEL_DEC2_M2_ORD_3RD		(2 << 1)
#define IMU_ACCEL_UI_FILT_ORD_1ST		0
#define IMU_ACCEL_UI_FILT_ORD_2ND		(1 << 3)
#define IMU_ACCEL_UI_FILT_ORD_3RD		(2 << 3)

// TMST_CONFIG (0x54)
#define IMU_TMST_DIS					0
#define IMU_TMST_EN						1
#define IMU_TMST_FSYNC_DIS				0
#define IMU_TMST_FSYNC_EN				(1 << 1)
#define IMU_TMST_DELTA_EN 				(1 << 2)
#define IMU_TMST_DELTA_DIS				0
#define IMU_TMST_RES_1US				0
#define IMU_TMST_RES_16US_OR_RTC		(1 << 3)
#define IMU_TMST_TO_REGS_DIS 			0
#define IMU_TMST_TO_REGS_EN 			(1 << 4)

// APEX_CONFIG0 (0x56)
#define IMU_DMP_ODR_25HZ				0
#define IMU_DMP_ODR_50HZ				2
#define IMU_R2W_EN						(1 << 3)
#define IMU_TILT_ENABLE					(1 << 4)
#define IMU_PED_ENABLE					(1 << 5)
#define IMU_TAP_ENABLE					(1 << 6)
#define IMU_DMP_POWER_SAVE_DIS			0
#define IMU_DMP_POWER_SAVE_EN			(1 << 7)

// SMD_CONFIG (0x57)
#define IMU_SMD_MODE_DISABLED			0
#define IMU_SMD_MODE_SHORT				2
#define IMU_SMD_MODE_LONG				3
#define IMU_WOM_MODE_INITIAL			0
#define IMU_WOM_MODE_PREVIOUS			(1 << 2)
#define IMU_WOM_INT_MODE_OR				0
#define IMU_WOM_INT_MODE_AND			(1 << 3)

// FIFO_CONFIG1 (0x5F)
#define IMU_FIFO_ACCEL_EN				1
#define IMU_FIFO_GYRO_EN 				(1 << 1)
#define IMU_FIFO_TEMP_EN				(1 << 2)
#define IMU_FIFO_TMST_FSYNC_EN			(1 << 3)
#define IMU_FIFO_HIRES_EN				(1 << 4)
#define IMU_FIFO_WM_GT_TH_DIS			0
#define IMU_FIFO_WM_GT_TH_EN			(1 << 5)
#define IMU_FIFO_RESUME_PARTIAL_RD_DIS	0
#define IMU_FIFO_RESUME_PARTIAL_RD_EN	(1 << 6)

// FIFO_CONFIG2 (0x60)
// FIFO_CONFIG3 (0x61)

// FSYNC_CONFIG (0x62)
#define IMU_FSYNC_POLARITY_RE			0
#define IMU_FSYNC_POLARITY_FE			1
#define IMU_FSYNC_UI_FLAG_CLEAR_SEL_UPD	0
#define IMU_FSYNC_UI_FLAG_CLEAR_SEL_R	(1 << 1)
#define IMU_FSYNC_UI_SEL_NONE			0
#define IMU_FSYNC_UI_SEL_TEMP_OUT		(1 << 4)
#define IMU_FSYNC_UI_SEL_GYRO_XOUT		(2 << 4)
#define IMU_FSYNC_UI_SEL_GYRO_YOUT		(3 << 4)
#define IMU_FSYNC_UI_SEL_GYRO_ZOUT		(4 << 4)
#define IMU_FSYNC_UI_SEL_ACCEL_XOUT		(5 << 4)
#define IMU_FSYNC_UI_SEL_ACCEL_YOUT		(6 << 4)
#define IMU_FSYNC_UI_SEL_ACCEL_ZOUT		(7 << 4)

// INT_CONFIG0 (0x63)
#define IMU_FIFO_FULL_INT_CLEAR_STATUSBIT	1
#define IMU_FIFO_FULL_INT_CLEAR_FIFO1B		2
#define IMU_FIFO_FULL_INT_CLEAR_AND			3
#define IMU_FIFO_THS_INT_CLEAR_STATUSBIT	(1 << 2)
#define IMU_FIFO_THS_INT_CLEAR_FIFO1B		(2 << 2)
#define IMU_FIFO_THS_INT_CLEAR_AND			(3 << 2)
#define IMU_UI_DRDY_INT_CLEAR_STATUSBIT		(1 << 4)
#define IMU_UI_DRDY_INT_CLEAR_REGREAD		(2 << 4)
#define IMU_UI_DRDY_INT_CLEAR_AND			(3 << 4)

// INT_CONFIG1 (0x64)
#define IMU_INT_ASYNC_RESET				(1 << 4)	// Must apply for proper INT1 and INT2 behavior
#define IMU_INT_TDEASSERT_DISABLE		(1 << 5)	// Required if ODR >= 4kHz, optional otherwise
#define IMU_INT_TPULSE_DURATION_100US	0			// Use only if ODR < 4kHz
#define IMU_INT_TPULSE_DURATION_8US		(1 << 6)	// Required if ODR >= 4kHz, optional otherwise

// INT_SOURCE0 (0x65)
#define IMU_UI_AGC_RDY_INT1_EN 			1
#define IMU_FIFO_FULL_INT1_EN			(1 << 1)
#define IMU_FIFO_THS_INT1_EN			(1 << 2)
#define IMU_UI_DRDY_INT1_EN				(1 << 3)
#define IMU_RESET_DONE_INT1_EN			(1 << 4)
#define IMU_PLL_RDY_INT1_EN				(1 << 5)
#define IMU_UI_FSYNC_INT1_EN			(1 << 6)

// INT_SOURCE1 (0x66)
#define IMU_WOM_X_INT1_EN				1
#define IMU_WOM_Y_INT1_EN				(1 << 1)
#define IMU_WOM_Z_INT1_EN				(1 << 2)
#define IMU_SMD_INT1_EN					(1 << 3)
#define IMU_I3C_PROTOCOL_ERROR_INT1_EN	(1 << 6)

// INT_SOURCE3 (0x68)
#define IMU_UI_AGC_RDY_INT2_EN 			1
#define IMU_FIFO_FULL_INT2_EN			(1 << 1)
#define IMU_FIFO_THS_INT2_EN			(1 << 2)
#define IMU_UI_DRDY_INT2_EN				(1 << 3)
#define IMU_RESET_DONE_INT2_EN			(1 << 4)
#define IMU_PLL_RDY_INT2_EN				(1 << 5)
#define IMU_UI_FSYNC_INT2_EN			(1 << 6)

// INT_SOURCE4 (0x69)
#define IMU_WOM_X_INT2_EN				1
#define IMU_WOM_Y_INT2_EN				(1 << 1)
#define IMU_WOM_Z_INT2_EN				(1 << 2)
#define IMU_SMD_INT2_EN					(1 << 3)
#define IMU_I3C_PROTOCOL_ERROR_INT2_EN	(1 << 6)

// FIFO_LOST_PKT0 (0x6C)
// FIFO_LOST_PKT1 (0x6D)

// SELF_TEST_CONFIG (0x70)
#define IMU_EN_GX_ST 					1
#define IMU_EN_GY_ST					(1 << 1)
#define IMU_EN_GZ_ST					(1 << 2)
#define IMU_EN_AX_ST					(1 << 3)
#define IMU_EN_AY_ST					(1 << 4)
#define IMU_EN_AZ_ST					(1 << 5)
#define IMU_ACCEL_ST_POWER_EN			(1 << 6)

// WHO_AM_I (Ox75)
#define IMU_WHOAMI						0x47

// REG_BANK_SEL (0x76)
#define IMU_BANK_SEL_0					0
#define IMU_BANK_SEL_1					1
#define IMU_BANK_SEL_2					2
#define IMU_BANK_SEL_3					3
#define IMU_BANK_SEL_4					4

// #################################### USR Bank 1
// SENSOR_CONFIG0 (0x03)
#define IMU_XA_DISABLE					1
#define IMU_YA_DISABLE					(1 << 1)
#define IMU_ZA_DISABLE					(1 << 2)
#define IMU_XG_DISABLE					(1 << 3)
#define IMU_YG_DISABLE					(1 << 4)
#define IMU_ZG_DISABLE					(1 << 5)

// GYRO_CONFIG_STATIC2 (0x0B)
#define IMU_GYRO_NF_EN					0
#define IMU_GYRO_NF_DIS					1
#define IMU_GYRO_AAF_EN					0
#define IMU_GYRO_AAF_DIS				(1 << 1)

// GYRO_CONFIG_STATIC3 (0x0C)
#define IMU_GYRO_AAF_DELT_42HZ			1
#define IMU_GYRO_AAF_DELT_84HZ			2
#define IMU_GYRO_AAF_DELT_126HZ			3
#define IMU_GYRO_AAF_DELT_170HZ			4
#define IMU_GYRO_AAF_DELT_213HZ			5
#define IMU_GYRO_AAF_DELT_258HZ			6
#define IMU_GYRO_AAF_DELT_303HZ			7
#define IMU_GYRO_AAF_DELT_348HZ			8
#define IMU_GYRO_AAF_DELT_394HZ			9
#define IMU_GYRO_AAF_DELT_441HZ			10
#define IMU_GYRO_AAF_DELT_488HZ			11
#define IMU_GYRO_AAF_DELT_536HZ			12
#define IMU_GYRO_AAF_DELT_585HZ			13
#define IMU_GYRO_AAF_DELT_634HZ			14
#define IMU_GYRO_AAF_DELT_684HZ			15
#define IMU_GYRO_AAF_DELT_734HZ			16
#define IMU_GYRO_AAF_DELT_785HZ			17
#define IMU_GYRO_AAF_DELT_837HZ			18
#define IMU_GYRO_AAF_DELT_890HZ			19
#define IMU_GYRO_AAF_DELT_943HZ			20
#define IMU_GYRO_AAF_DELT_997HZ			21
#define IMU_GYRO_AAF_DELT_1051HZ		22
#define IMU_GYRO_AAF_DELT_1107HZ		23
#define IMU_GYRO_AAF_DELT_1163HZ		24
#define IMU_GYRO_AAF_DELT_1220HZ		25
#define IMU_GYRO_AAF_DELT_1277HZ		26
#define IMU_GYRO_AAF_DELT_1336HZ		27
#define IMU_GYRO_AAF_DELT_1395HZ		28
#define IMU_GYRO_AAF_DELT_1454HZ		29
#define IMU_GYRO_AAF_DELT_1515HZ		30
#define IMU_GYRO_AAF_DELT_1577HZ		31
#define IMU_GYRO_AAF_DELT_1639HZ		32
#define IMU_GYRO_AAF_DELT_1702HZ		33
#define IMU_GYRO_AAF_DELT_1766HZ		34
#define IMU_GYRO_AAF_DELT_1830HZ		35
#define IMU_GYRO_AAF_DELT_1896HZ		36
#define IMU_GYRO_AAF_DELT_1962HZ		37
#define IMU_GYRO_AAF_DELT_2029HZ		38
#define IMU_GYRO_AAF_DELT_2097HZ		39
#define IMU_GYRO_AAF_DELT_2166HZ		40
#define IMU_GYRO_AAF_DELT_2235HZ		41
#define IMU_GYRO_AAF_DELT_2306HZ		42
#define IMU_GYRO_AAF_DELT_2377HZ		43
#define IMU_GYRO_AAF_DELT_2449HZ		44
#define IMU_GYRO_AAF_DELT_2522HZ		45
#define IMU_GYRO_AAF_DELT_2596HZ		46
#define IMU_GYRO_AAF_DELT_2671HZ		47
#define IMU_GYRO_AAF_DELT_2746HZ		48
#define IMU_GYRO_AAF_DELT_2823HZ		49
#define IMU_GYRO_AAF_DELT_2900HZ		50
#define IMU_GYRO_AAF_DELT_2978HZ		51
#define IMU_GYRO_AAF_DELT_3057HZ		52
#define IMU_GYRO_AAF_DELT_3137HZ		53
#define IMU_GYRO_AAF_DELT_3217HZ		54
#define IMU_GYRO_AAF_DELT_3299HZ		55
#define IMU_GYRO_AAF_DELT_3381HZ		56
#define IMU_GYRO_AAF_DELT_3464HZ		57
#define IMU_GYRO_AAF_DELT_3548HZ		58
#define IMU_GYRO_AAF_DELT_3633HZ		59
#define IMU_GYRO_AAF_DELT_3718HZ		60
#define IMU_GYRO_AAF_DELT_3805HZ		61
#define IMU_GYRO_AAF_DELT_3892HZ		62
#define IMU_GYRO_AAF_DELT_3979HZ		63

// GYRO_CONFIG_STATIC4 (0x0D) and GYRO_CONFIG_STATIC5 (0x0E)
#define IMU_GYRO_AAF_DELTSQR(GYRO_AAF_DELT)		(	GYRO_AAF_DELT <= 10 ? (GYRO_AAF_DELT * GYRO_AAF_DELT) : \
													GYRO_AAF_DELT == 11 ? 122 : \
													GYRO_AAF_DELT == 12 ? 144 : \
													GYRO_AAF_DELT == 13 ? 170 : \
													GYRO_AAF_DELT == 14 ? 196 : \
													GYRO_AAF_DELT == 15 ? 224 : \
													GYRO_AAF_DELT == 16 ? 256 : \
													GYRO_AAF_DELT == 17 ? 288 : \
													GYRO_AAF_DELT == 18 ? 324 : \
													GYRO_AAF_DELT == 19 ? 360 : \
													GYRO_AAF_DELT == 20 ? 400 : \
													GYRO_AAF_DELT == 21 ? 440 : \
													GYRO_AAF_DELT == 22 ? 488 : \
													GYRO_AAF_DELT == 23 ? 528 : \
													GYRO_AAF_DELT == 24 ? 576 : \
													GYRO_AAF_DELT == 25 ? 624 : \
													GYRO_AAF_DELT == 26 ? 680 : \
													GYRO_AAF_DELT == 27 ? 736 : \
													GYRO_AAF_DELT == 28 ? 784 : \
													GYRO_AAF_DELT == 29 ? 848 : \
													GYRO_AAF_DELT == 30 ? 896 : \
													GYRO_AAF_DELT == 31 ? 960 : \
													GYRO_AAF_DELT == 32 ? 1024 : \
													GYRO_AAF_DELT == 33 ? 1088 : \
													GYRO_AAF_DELT == 34 ? 1152 : \
													GYRO_AAF_DELT == 35 ? 1232 : \
													GYRO_AAF_DELT == 36 ? 1296 : \
													GYRO_AAF_DELT == 37 ? 1376 : \
													GYRO_AAF_DELT == 38 ? 1440 : \
													GYRO_AAF_DELT == 39 ? 1536 : \
													GYRO_AAF_DELT == 40 ? 1600 : \
													GYRO_AAF_DELT == 41 ? 1696 : \
													GYRO_AAF_DELT == 42 ? 1760 : \
													GYRO_AAF_DELT == 43 ? 1856 : \
													GYRO_AAF_DELT == 44 ? 1952 : \
													GYRO_AAF_DELT == 45 ? 2016 : \
													GYRO_AAF_DELT == 46 ? 2112 : \
													GYRO_AAF_DELT == 47 ? 2208 : \
													GYRO_AAF_DELT == 48 ? 2304 : \
													GYRO_AAF_DELT == 49 ? 2400 : \
													GYRO_AAF_DELT == 50 ? 2496 : \
													GYRO_AAF_DELT == 51 ? 2592 : \
													GYRO_AAF_DELT == 52 ? 2720 : \
													GYRO_AAF_DELT == 53 ? 2816 : \
													GYRO_AAF_DELT == 54 ? 2944 : \
													GYRO_AAF_DELT == 55 ? 3008 : \
													GYRO_AAF_DELT == 56 ? 3136 : \
													GYRO_AAF_DELT == 57 ? 3264 : \
													GYRO_AAF_DELT == 58 ? 3392 : \
													GYRO_AAF_DELT == 59 ? 3456 : \
													GYRO_AAF_DELT == 60 ? 3584 : \
													GYRO_AAF_DELT == 61 ? 3712 : \
													GYRO_AAF_DELT == 62 ? 3840 : \
													GYRO_AAF_DELT == 63 ? 3968 : 0)
#define IMU_GYRO_AAF_BITSHIFT(GYRO_AAF_DELT)	((	GYRO_AAF_DELT < 2 ? 15 : \
													GYRO_AAF_DELT < 3 ? 13 : \
													GYRO_AAF_DELT < 4 ? 12 : \
													GYRO_AAF_DELT < 5 ? 11 : \
													GYRO_AAF_DELT < 7 ? 10 : \
													GYRO_AAF_DELT < 10 ? 9 : \
													GYRO_AAF_DELT < 14 ? 8 : \
													GYRO_AAF_DELT < 19 ? 7 : \
													GYRO_AAF_DELT < 27 ? 6 : \
													GYRO_AAF_DELT < 37 ? 5 : \
													GYRO_AAF_DELT < 53 ? 4 : 3) << 4)

// GYRO_CONFIG_STATIC6 (0x0F) to GYRO_CONFIG_STATIC10 (0x13)
#define IMU_GYRO_NF_BW_SEL_1449HZ		0
#define IMU_GYRO_NF_BW_SEL_680HZ		(1 << 4)
#define IMU_GYRO_NF_BW_SEL_329HZ		(2 << 4)
#define IMU_GYRO_NF_BW_SEL_162HZ		(3 << 4)
#define IMU_GYRO_NF_BW_SEL_80HZ			(4 << 4)
#define IMU_GYRO_NF_BW_SEL_40HZ			(5 << 4)
#define IMU_GYRO_NF_BW_SEL_20HZ			(6 << 4)
#define IMU_GYRO_NF_BW_SEL_10HZ			(7 << 4)

// XG_ST_DATA (0x5F)
// YG_ST_DATA (0x60)
// ZG_ST_DATA (0x61)
// TMSTVAL0 (0x62)
// TMSTVAL1 (0x63)
// TMSTVAL2 (0x64)

// INTF_CONFIG4 (0x7A)
#define IMU_SPI_AP_3WIRE				0
#define IMU_SPI_AP_4WIRE				(1 << 1)
#define IMU_I3C_BUS_MODE_I2C			0
#define IMU_I3C_BUS_MODE_I3C			(1 << 6)

// INTF_CONFIG5 (Ox7B)
#define IMU_PIN9_FUNCTION_INT2			0
#define IMU_PIN9_FUNCTION_FSYNC			(1 << 1)
#define IMU_PIN9_FUNCTION_CLKIN			(2 << 1)

// INTF_CONFIG6 (0x7C)
#define IMU_I3C_SDR_EN					1
#define IMU_I3C_DDR_EN					(1 << 1)
#define IMU_I3C_IBI_EN					(1 << 2)
#define IMU_I3C_IBI_BYTE_EN				(1 << 3)
#define IMU_I3C_EN						(1 << 4)
#define IMU_ASYNCTIME0_DIS				(1 << 7)

// #################################### USR Bank 2
// ACCEL_CONFIG_STATIC2 (0x03)
#define IMU_ACCEL_AAF_EN				0
#define IMU_ACCEL_AAF_DIS				1
#define IMU_ACCEL_AAF_DELT_42HZ			(1 << 1)
#define IMU_ACCEL_AAF_DELT_84HZ			(2 << 1)
#define IMU_ACCEL_AAF_DELT_126HZ		(3 << 1)
#define IMU_ACCEL_AAF_DELT_170HZ		(4 << 1)
#define IMU_ACCEL_AAF_DELT_213HZ		(5 << 1)
#define IMU_ACCEL_AAF_DELT_258HZ		(6 << 1)
#define IMU_ACCEL_AAF_DELT_303HZ		(7 << 1)
#define IMU_ACCEL_AAF_DELT_348HZ		(8 << 1)
#define IMU_ACCEL_AAF_DELT_394HZ		(9 << 1)
#define IMU_ACCEL_AAF_DELT_441HZ		(10 << 1)
#define IMU_ACCEL_AAF_DELT_488HZ		(11 << 1)
#define IMU_ACCEL_AAF_DELT_536HZ		(12 << 1)
#define IMU_ACCEL_AAF_DELT_585HZ		(13 << 1)
#define IMU_ACCEL_AAF_DELT_634HZ		(14 << 1)
#define IMU_ACCEL_AAF_DELT_684HZ		(15 << 1)
#define IMU_ACCEL_AAF_DELT_734HZ		(16 << 1)
#define IMU_ACCEL_AAF_DELT_785HZ		(17 << 1)
#define IMU_ACCEL_AAF_DELT_837HZ		(18 << 1)
#define IMU_ACCEL_AAF_DELT_890HZ		(19 << 1)
#define IMU_ACCEL_AAF_DELT_943HZ		(20 << 1)
#define IMU_ACCEL_AAF_DELT_997HZ		(21 << 1)
#define IMU_ACCEL_AAF_DELT_1051HZ		(22 << 1)
#define IMU_ACCEL_AAF_DELT_1107HZ		(23 << 1)
#define IMU_ACCEL_AAF_DELT_1163HZ		(24 << 1)
#define IMU_ACCEL_AAF_DELT_1220HZ		(25 << 1)
#define IMU_ACCEL_AAF_DELT_1277HZ		(26 << 1)
#define IMU_ACCEL_AAF_DELT_1336HZ		(27 << 1)
#define IMU_ACCEL_AAF_DELT_1395HZ		(28 << 1)
#define IMU_ACCEL_AAF_DELT_1454HZ		(29 << 1)
#define IMU_ACCEL_AAF_DELT_1515HZ		(30 << 1)
#define IMU_ACCEL_AAF_DELT_1577HZ		(31 << 1)
#define IMU_ACCEL_AAF_DELT_1639HZ		(32 << 1)
#define IMU_ACCEL_AAF_DELT_1702HZ		(33 << 1)
#define IMU_ACCEL_AAF_DELT_1766HZ		(34 << 1)
#define IMU_ACCEL_AAF_DELT_1830HZ		(35 << 1)
#define IMU_ACCEL_AAF_DELT_1896HZ		(36 << 1)
#define IMU_ACCEL_AAF_DELT_1962HZ		(37 << 1)
#define IMU_ACCEL_AAF_DELT_2029HZ		(38 << 1)
#define IMU_ACCEL_AAF_DELT_2097HZ		(39 << 1)
#define IMU_ACCEL_AAF_DELT_2166HZ		(40 << 1)
#define IMU_ACCEL_AAF_DELT_2235HZ		(41 << 1)
#define IMU_ACCEL_AAF_DELT_2306HZ		(42 << 1)
#define IMU_ACCEL_AAF_DELT_2377HZ		(43 << 1)
#define IMU_ACCEL_AAF_DELT_2449HZ		(44 << 1)
#define IMU_ACCEL_AAF_DELT_2522HZ		(45 << 1)
#define IMU_ACCEL_AAF_DELT_2596HZ		(46 << 1)
#define IMU_ACCEL_AAF_DELT_2671HZ		(47 << 1)
#define IMU_ACCEL_AAF_DELT_2746HZ		(48 << 1)
#define IMU_ACCEL_AAF_DELT_2823HZ		(49 << 1)
#define IMU_ACCEL_AAF_DELT_2900HZ		(50 << 1)
#define IMU_ACCEL_AAF_DELT_2978HZ		(51 << 1)
#define IMU_ACCEL_AAF_DELT_3057HZ		(52 << 1)
#define IMU_ACCEL_AAF_DELT_3137HZ		(53 << 1)
#define IMU_ACCEL_AAF_DELT_3217HZ		(54 << 1)
#define IMU_ACCEL_AAF_DELT_3299HZ		(55 << 1)
#define IMU_ACCEL_AAF_DELT_3381HZ		(56 << 1)
#define IMU_ACCEL_AAF_DELT_3464HZ		(57 << 1)
#define IMU_ACCEL_AAF_DELT_3548HZ		(58 << 1)
#define IMU_ACCEL_AAF_DELT_3633HZ		(59 << 1)
#define IMU_ACCEL_AAF_DELT_3718HZ		(60 << 1)
#define IMU_ACCEL_AAF_DELT_3805HZ		(61 << 1)
#define IMU_ACCEL_AAF_DELT_3892HZ		(62 << 1)
#define IMU_ACCEL_AAF_DELT_3979HZ		(63 << 1)

// ACCEL_CONFIG_STATIC3 (0x04) and ACCEL_CONFIG_STATIC4 (0x05)
#define IMU_ACCEL_AAF_DELTSQR(ACCEL_AAF_DELT)	(IMU_GYRO_AAF_DELTSQR(ACCEL_AAF_DELT >> 1))
#define IMU_ACCEL_AAF_BITSHIFT(ACCEL_AAF_DELT)	(IMU_GYRO_AAF_BITSHIFT(ACCEL_AAF_DELT))

// XA_ST_DATA (0x3B)
// YA_ST_DATA (0x3C)
// ZA_ST_DATA (0x3D)

// #################################### USR Bank 3
// CLKDIV (0x2A)
#define IMU_CLKDIV_1_TO_127(VALUE)		((VALUE >= 1 && VALUE <= 127) ? VALUE : 0)

// #################################### USR Bank 4
// APEX_CONFIG1 (0x40)
#define IMU_DMP_POWER_SAVE_TIME_SEL_0S		0
#define IMU_DMP_POWER_SAVE_TIME_SEL_4S		1
#define IMU_DMP_POWER_SAVE_TIME_SEL_8S		2
#define IMU_DMP_POWER_SAVE_TIME_SEL_12S		3
#define IMU_DMP_POWER_SAVE_TIME_SEL_16S		4
#define IMU_DMP_POWER_SAVE_TIME_SEL_20S		5
#define IMU_DMP_POWER_SAVE_TIME_SEL_24S		6
#define IMU_DMP_POWER_SAVE_TIME_SEL_28S		7
#define IMU_DMP_POWER_SAVE_TIME_SEL_32S		8
#define IMU_DMP_POWER_SAVE_TIME_SEL_36S		9
#define IMU_DMP_POWER_SAVE_TIME_SEL_40S		10
#define IMU_DMP_POWER_SAVE_TIME_SEL_44S		11
#define IMU_DMP_POWER_SAVE_TIME_SEL_48S		12
#define IMU_DMP_POWER_SAVE_TIME_SEL_52S		13
#define IMU_DMP_POWER_SAVE_TIME_SEL_56S		14
#define IMU_DMP_POWER_SAVE_TIME_SEL_60S		15
#define IMU_LOW_ENERGY_AMP_TH_SEL_10		(10 << 4)	// Default, use this value.

// APEX_CONFIG2 (0x41)
#define IMU_PED_STEP_CNT_TH_SEL_0			0
#define IMU_PED_STEP_CNT_TH_SEL_1			1
#define IMU_PED_STEP_CNT_TH_SEL_2			2
#define IMU_PED_STEP_CNT_TH_SEL_3			3
#define IMU_PED_STEP_CNT_TH_SEL_4			4
#define IMU_PED_STEP_CNT_TH_SEL_5			5
#define IMU_PED_STEP_CNT_TH_SEL_6			6
#define IMU_PED_STEP_CNT_TH_SEL_7			7
#define IMU_PED_STEP_CNT_TH_SEL_8			8
#define IMU_PED_STEP_CNT_TH_SEL_9			9
#define IMU_PED_STEP_CNT_TH_SEL_10			10
#define IMU_PED_STEP_CNT_TH_SEL_11			11
#define IMU_PED_STEP_CNT_TH_SEL_12			12
#define IMU_PED_STEP_CNT_TH_SEL_13			13
#define IMU_PED_STEP_CNT_TH_SEL_14			14
#define IMU_PED_STEP_CNT_TH_SEL_15			15
#define IMU_PED_AMP_TH_SEL_8				(8 << 4)	// Default, use this value.

// APEX_CONFIG3 (0x42)
#define IMU_PED_HI_EN_TH_SEL_1				1	// Default, use this value.
#define IMU_PED_SB_TIMER_TH_SEL_0			0
#define IMU_PED_SB_TIMER_TH_SEL_1			(1 << 2)
#define IMU_PED_SB_TIMER_TH_SEL_2			(2 << 2)
#define IMU_PED_SB_TIMER_TH_SEL_3			(3 << 2)
#define IMU_PED_SB_TIMER_TH_SEL_4			(4 << 2)
#define IMU_PED_SB_TIMER_TH_SEL_5			(5 << 2)
#define IMU_PED_SB_TIMER_TH_SEL_6			(6 << 2)
#define IMU_PED_SB_TIMER_TH_SEL_7			(7 << 2)
#define IMU_PED_STEP_DET_TH_SEL_2			(2 << 5)	// Default, use this value.

// APEX_CONFIG4 (0x43)
#define IMU_SLEEP_TIME_OUT_1_28S			0
#define IMU_SLEEP_TIME_OUT_2_56S			(1 << 3)
#define IMU_SLEEP_TIME_OUT_3_84S			(2 << 3)
#define IMU_SLEEP_TIME_OUT_5_12S			(3 << 3)
#define IMU_SLEEP_TIME_OUT_6_40S			(4 << 3)
#define IMU_SLEEP_TIME_OUT_7_68S			(5 << 3)
#define IMU_SLEEP_TIME_OUT_8_96S			(6 << 3)
#define IMU_SLEEP_TIME_OUT_10_24S			(7 << 3)
#define IMU_TILT_WAIT_TIME_SEL_0S			0
#define IMU_TILT_WAIT_TIME_SEL_2S			(1 << 6)
#define IMU_TILT_WAIT_TIME_SEL_4S			(2 << 6)
#define IMU_TILT_WAIT_TIME_SEL_6S			(3 << 6)

// APEX_CONFIG5 (0x44)
#define IMU_MOUNTING_MATRIX_0				0	// [1 0 0 ; 0 1 0 ; 0 0 1]
#define IMU_MOUNTING_MATRIX_1				1	// [1 0 0 ; 0 -1 0 ; 0 0 -1]
#define IMU_MOUNTING_MATRIX_2				2	// [-1 0 0 ; 0 1 0 ; 0 0 -1]
#define IMU_MOUNTING_MATRIX_3				3	// [-1 0 0 ; 0 -1 0 ; 0 0 1]
#define IMU_MOUNTING_MATRIX_4				4	// [0 1 0 ; 1 0 0 ; 0 0 -1]
#define IMU_MOUNTING_MATRIX_5				5	// [0 1 0 ; -1 0 0 ; 0 0 1]
#define IMU_MOUNTING_MATRIX_6				6	// [0 -1 0 ; 1 0 0 ; 0 0 1]
#define IMU_MOUNTING_MATRIX_7				7	// [0 -1 0 ; -1 0 0 ; 0 0 -1]

// APEX_CONFIG6 (0x45)
#define IMU_SLEEP_GESTURE_DELAY_0_32S		0
#define IMU_SLEEP_GESTURE_DELAY_0_64S		1
#define IMU_SLEEP_GESTURE_DELAY_0_96S		2
#define IMU_SLEEP_GESTURE_DELAY_1_28S		3
#define IMU_SLEEP_GESTURE_DELAY_1_60S		4
#define IMU_SLEEP_GESTURE_DELAY_1_92S		5
#define IMU_SLEEP_GESTURE_DELAY_2_24S		6
#define IMU_SLEEP_GESTURE_DELAY_2_56S		7

// APEX_CONFIG7 (0x46)
#define IMU_TAP_MAX_PEAK_TOL_1				1	// Default, use this value.
#define IMU_TAP_MIN_JERK_THR_17				(17 << 2)	// Default, use this value.

// APEX_CONFIG8 (0x47)
#define IMU_TAP_TMIN_3						3	// Default, use this value.
#define IMU_TAP_TAVG_1						(1 << 3)	// Default, use this value.
#define IMU_TAP_TMAX_1						(1 << 5)	// Default, use this value.

// APEX_CONFIG9 (0x48)
#define IMU_SENSITIVITY_MODE_NORMAL			0
#define IMU_SENSITIVITY_MODE_SLOWWALK		1

// ACCEL_WOM_X_THR (0x4A), ACCEL_WOM_Y_THR (0x4B) and ACCEL_WOM_Z_THR (0x4C)
static inline uint8_t IMU_ACCEL_WOM_AXIS_THR(float mgVal){
	float v = mgVal * 0.256f;
	if(v <= 0.0f) return 0;
	if(v >= 255.0f) return 255;
	return (uint8_t)v;
}

// INT_SOURCE6 (0x4D)
#define IMU_TAP_DET_INT1_EN					1
#define IMU_SLEEP_DET_INT1_EN				(1 << 1)
#define IMU_WAKE_DET_INT1_EN				(1 << 2)
#define IMU_TILT_DET_INT1_EN				(1 << 3)
#define IMU_STEP_CNT_OFL_INT1_EN			(1 << 4)
#define IMU_STEP_DET_INT1_EN				(1 << 5)

// INT_SOURCE7 (0x4E)
#define IMU_TAP_DET_INT2_EN					1
#define IMU_SLEEP_DET_INT2_EN				(1 << 1)
#define IMU_WAKE_DET_INT2_EN				(1 << 2)
#define IMU_TILT_DET_INT2_EN				(1 << 3)
#define IMU_STEP_CNT_OFL_INT2_EN			(1 << 4)
#define IMU_STEP_DET_INT2_EN				(1 << 5)

// INT_SOURCE8 (0x4F)
#define IMU_AGC_RDY_IBI_EN					1
#define IMU_FIFO_FULL_IBI_EN				(1 << 1)
#define IMU_FIFO_THS_IBI_EN					(1 << 2)
#define IMU_UI_DRDY_IBI_EN					(1 << 3)
#define IMU_PLL_RDY_IBI_EN					(1 << 4)
#define IMU_FSYNC_IBI_EN					(1 << 5)

// INT_SOURCE9 (0x50)
#define IMU_WOM_X_IBI_EN					(1 << 1)
#define IMU_WOM_Y_IBI_EN					(1 << 2)
#define IMU_WOM_Z_IBI_EN					(1 << 3)
#define IMU_SMD_IBI_EN						(1 << 4)
#define IMU_I3C_PROTOCOL_ERROR_IBI_EN		(1 << 7)

// INT_SOURCE10 (0x51)
#define IMU_TAP_DET_IBI_EN					1
#define IMU_SLEEP_DET_IBI_EN				(1 << 1)
#define IMU_WAKE_DET_IBI_EN					(1 << 2)
#define IMU_TILT_DET_IBI_EN					(1 << 3)
#define IMU_STEP_CNT_OFL_IBI_EN				(1 << 4)
#define IMU_STEP_DET_IBI_EN					(1 << 5)

// OFFSET_USER0 to OFFSET_USER8 (0x77 to 0x7F)
/*
 * IMU_GYRO_AXIS_OFFUSER
 * Sets related gyroscope offset bits in a buffer for OFFSET_USERx registers.
 * Offset range: +-64 dps
 * Resolution: 1/32 dps
 *
 * Parameters: X-axis offset (dps), Y-axis offset (dps), Z-axis offset (dps), Buffer array (uint8_t array of size 9)
 * Returns: -
 */
static inline void IMU_GYRO_AXIS_OFFUSER(float dpsValX, float dpsValY, float dpsValZ, uint8_t offsetArr[static 9]){
	float valArr[3] = {dpsValX, dpsValY, dpsValZ};
	uint16_t tempArr[3];
	for(int i = 0; i < 3; i++){
		if(valArr[i] > 63.9f) valArr[i] = 63.9f;		// Note: 63.9 is used instead of 64.0 against overflow risk.
		else if(valArr[i] < -64.0f) valArr[i] = -64.0f;
		int16_t lsb = (int16_t)(valArr[i] * 32.0f);
		tempArr[i] = (uint16_t)(lsb & 0x0FFF);
	}
	offsetArr[0] = (uint8_t)(tempArr[0] & 0xFF);
	offsetArr[1] = (uint8_t)(((tempArr[0] >> 8) & 0x0F) | ((tempArr[1] >> 4) & 0xF0));
	offsetArr[2] = (uint8_t)(tempArr[1] & 0xFF);
	offsetArr[3] = (uint8_t)(tempArr[2] & 0xFF);
	offsetArr[4] &= 0xF0;
	offsetArr[4] |= (uint8_t)((tempArr[2] >> 8) & 0x0F);
}

/*
 * IMU_ACCEL_AXIS_OFFUSER
 * Sets related accelerometer offset bits in a buffer for OFFSET_USERx registers.
 * Offset range: +-1000 mg
 * Resolution: 0.5 mg
 *
 * Parameters: X-axis offset (mg), Y-axis offset (mg), Z-axis offset (mg), Buffer array (uint8_t array of size 9)
 * Returns: -
 */
static inline void IMU_ACCEL_AXIS_OFFUSER(float mgValX, float mgValY, float mgValZ, uint8_t offsetArr[static 9]){
	float valArr[3] = {mgValX, mgValY, mgValZ};
	uint16_t tempArr[3];
	for(int i = 0; i < 3; i++){
		if(valArr[i] > 1000.0f) valArr[i] = 1000.0f;
		else if(valArr[i] < -1000.0f) valArr[i] = -1000.0f;
		int16_t lsb = (int16_t)(valArr[i] * 2.0f);
		tempArr[i] = (uint16_t)(lsb & 0x0FFF);
	}
	offsetArr[4] &= 0x0F;
	offsetArr[4] |= (uint8_t)((tempArr[0] >> 4) & 0xF0);
	offsetArr[5] = (uint8_t)(tempArr[0] & 0xFF);
	offsetArr[6] = (uint8_t)(tempArr[1] & 0xFF);
	offsetArr[7] = (uint8_t)(((tempArr[1] >> 8) & 0x0F) | ((tempArr[2] >> 4) & 0xF0));
	offsetArr[8] = (uint8_t)(tempArr[2] & 0xFF);
}

/*#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################*/



/*	#############################################
 * 	          Structure Type Definitions
 *	############################################# */

// #################################### IMU Register Configuration Structure
typedef struct{
	// UI and Power Configuration
	uint8_t INTERFACE_x;
	uint8_t SENSOR_DATA_ENDIAN_x;
	// Accelerometer Configuration
	// Gyroscope Configuration
	// Temperature Sensor Configuration
	// FIFO Configuration
	uint8_t FIFO_MODE_x;
	uint8_t FIFO_DATA_ENDIAN_x;
	// SPI Configuration
	uint8_t SPI_MODE_x;
	uint8_t SPI_SLEW_RATE_x;
	// I2C and I3C Configuration
	// Interrupt Configuration
	uint8_t INT1_POLARITY_x;
	uint8_t INT1_DRIVE_CIRCUIT_x;
	uint8_t INT1_MODE_x;
	uint8_t INT2_POLARITY_x;
	uint8_t INT2_DRIVE_CIRCUIT_x;
	uint8_t INT2_MODE_x;
}IMU_Config_t;

// #################################### IMU Handler Structure
typedef struct{
	// Variables needed to be assigned by the user before IMU_Init function call
	IMU_Config_t* pIMU;
	SPI_HandleTypeDef* pSPIx;
	GPIO_TypeDef* pGPIOx;
	uint16_t GPIO_PIN_x;

	// Variables assigned by the IMU_Init function
	float accelMult;
	float gyroMult;
}IMU_Handler_t;

// #################################### IMU Data Structure
typedef struct{
	float accelX;
	float accelY;
	float accelZ;
	float gyroX;
	float gyroY;
	float gyroZ;
	float tempC;
}IMU_Data_t;

/*#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################
 *#############################################################################*/

/*	#############################################
 * 	         	Function Prototypes
 *	############################################# */

IMU_Status_t IMU_SPI_ReadReg(IMU_Handler_t* imu, uint8_t REG, uint8_t* rxBuffer);
IMU_Status_t IMU_SPI_WriteReg(IMU_Handler_t* imu, uint8_t REG, uint8_t txData);
IMU_Status_t IMU_SPI_ReadBurst(IMU_Handler_t* imu, uint8_t REG, uint8_t* rxBuffer, uint8_t size);
IMU_Status_t IMU_SPI_WriteBurst(IMU_Handler_t* imu, uint8_t REG, uint8_t* txBuffer, uint8_t size);
IMU_Status_t IMU_SPI_Init(IMU_Handler_t* imu);
IMU_Status_t IMU_SPI_ReadAccel(IMU_Handler_t* imu, IMU_Data_t* data);
IMU_Status_t IMU_SPI_ReadGyro(IMU_Handler_t* imu, IMU_Data_t* data);
IMU_Status_t IMU_SPI_ReadTemp(IMU_Handler_t* imu, IMU_Data_t* data);
IMU_Status_t IMU_SPI_GetData(IMU_Handler_t* imu, IMU_Data_t* data);
IMU_Status_t IMU_SPI_ReadFIFO(IMU_Handler_t* imu, IMU_Data_t* data);



#endif /* INC_IMU_H_ */
