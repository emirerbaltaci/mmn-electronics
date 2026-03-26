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

#ifndef DRIVERS_INC_HYDROPHONE_H_
#define DRIVERS_INC_HYDROPHONE_H_

#include "stm32g4xx_hal.h"
#include "arm_math.h"
#include <stdint.h>
#include <stdbool.h>

/* ── Configuration ─────────────────────────────────────────────────── */

#define HYDRO_FFT_SIZE          256
#define HYDRO_SAMPLE_RATE_HZ    100000.0f

/* Frequency resolution = SAMPLE_RATE / FFT_SIZE  ≈ 97.6 Hz per bin  */
#define HYDRO_FREQ_RES_HZ       (HYDRO_SAMPLE_RATE_HZ / (float32_t)HYDRO_FFT_SIZE)

/* Bytes per sample (16-bit ADC over 8-bit SPI = 2 bytes)             */
#define HYDRO_BYTES_PER_SAMPLE  2
/* Total DMA buffer size in bytes (double-buffered)                   */
#define HYDRO_DMA_BUF_SIZE      (HYDRO_FFT_SIZE * HYDRO_BYTES_PER_SAMPLE * 2)

/* Detection threshold for peak magnitude (tune empirically)          */
#define HYDRO_DETECT_THRESHOLD  200.0f

/* Clipping threshold — 12-bit ADC max                                */
#define HYDRO_ADC_MAX           4095

/* ── Types ─────────────────────────────────────────────────────────── */

typedef enum {
	HYDRO_OK = 0,
	HYDRO_ERROR,
	HYDRO_BUSY,
	HYDRO_NO_DATA
} Hydro_Status_t;

typedef enum {
	HYDRO_BUF_FIRST_HALF = 0,
	HYDRO_BUF_SECOND_HALF
} Hydro_BufferHalf_t;

typedef struct {
	/* Peripheral                                                      */
	SPI_HandleTypeDef          *hspi;
	GPIO_TypeDef               *cs_port;
	uint16_t                    cs_pin;

	/* Raw byte double-buffer (DMA fills this circularly)             */
	uint8_t                     dma_buffer[HYDRO_DMA_BUF_SIZE];

	/* Processing buffers                                              */
	float32_t                   fft_input[HYDRO_FFT_SIZE];
	float32_t                   fft_output[HYDRO_FFT_SIZE];
	float32_t                   magnitudes[HYDRO_FFT_SIZE / 2];
	float32_t                   window[HYDRO_FFT_SIZE];

	/* CMSIS-DSP FFT instance                                          */
	arm_rfft_fast_instance_f32  fft_instance;

	/* Results                                                         */
	uint32_t                    dominant_freq_hz;
	float32_t                   peak_magnitude;

	/* Status flags                                                    */
	volatile bool               data_ready;
	bool                        signal_detected;
	bool                        clipping;
} Hydro_Handler_t;

/* ── API ───────────────────────────────────────────────────────────── */

Hydro_Status_t Hydro_Init(Hydro_Handler_t *h, SPI_HandleTypeDef *hspi,
                          GPIO_TypeDef *cs_port, uint16_t cs_pin);
Hydro_Status_t Hydro_StartReceive(Hydro_Handler_t *h);
Hydro_Status_t Hydro_Process(Hydro_Handler_t *h);

/* Call these from the HAL SPI Rx callbacks in main.c or stm32g4xx_it.c */
void Hydro_SPI_RxHalfCpltCallback(Hydro_Handler_t *h, SPI_HandleTypeDef *hspi);
void Hydro_SPI_RxCpltCallback(Hydro_Handler_t *h, SPI_HandleTypeDef *hspi);

#endif /* DRIVERS_INC_HYDROPHONE_H_ */
