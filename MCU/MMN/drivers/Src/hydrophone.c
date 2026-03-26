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

#include "hydrophone.h"
#include <string.h>

/* ── Private helpers ───────────────────────────────────────────────── */

/**
 * @brief  Copy raw 16-bit samples from a DMA buffer half into the
 *         float processing buffer and check for clipping.
 * @param  h      Hydrophone handler
 * @param  half   Which half of the DMA double-buffer to read
 */
static void Hydro_LoadSamples(Hydro_Handler_t *h, Hydro_BufferHalf_t half)
{
	const uint8_t *src;
	uint8_t clip = 0;
	const uint32_t half_bytes = HYDRO_FFT_SIZE * HYDRO_BYTES_PER_SAMPLE;

	if (half == HYDRO_BUF_FIRST_HALF) {
		src = &h->dma_buffer[0];
	} else {
		src = &h->dma_buffer[half_bytes];
	}

	for (int i = 0; i < HYDRO_FFT_SIZE; i++) {
		/* Reassemble 16-bit sample from 2 bytes (MSB first) */
		uint16_t sample = ((uint16_t)src[i * 2] << 8) | src[i * 2 + 1];
		h->fft_input[i] = (float32_t)sample;
		if (sample == 0 || sample >= HYDRO_ADC_MAX) {
			clip = 1;
		}
	}

	h->clipping = (clip == 1);
	h->data_ready = true;
}

/* ── Public API ────────────────────────────────────────────────────── */

/**
 * @brief  Initialize the hydrophone processing pipeline.
 *
 *         Sets up the CMSIS-DSP FFT instance and pre-computes the
 *         Hanning window coefficients.  Does NOT start SPI reception;
 *         call Hydro_StartReceive() after peripheral clocks are ready.
 *
 * @param  h     Pointer to an uninitialized Hydro_Handler_t
 * @param  hspi  HAL SPI handle connected to the acoustic controller
 * @return HYDRO_OK on success
 */
Hydro_Status_t Hydro_Init(Hydro_Handler_t *h, SPI_HandleTypeDef *hspi,
                          GPIO_TypeDef *cs_port, uint16_t cs_pin)
{
	h->hspi    = hspi;
	h->cs_port = cs_port;
	h->cs_pin  = cs_pin;

	/* Deassert CS (idle high) */
	HAL_GPIO_WritePin(h->cs_port, h->cs_pin, GPIO_PIN_SET);

	/* Clear state */
	h->data_ready       = false;
	h->signal_detected  = false;
	h->clipping         = false;
	h->dominant_freq_hz = 0;
	h->peak_magnitude   = 0.0f;
	memset(h->dma_buffer, 0, sizeof(h->dma_buffer));

	/* Initialize CMSIS-DSP FFT (real-valued, 1024-point) */
	if (arm_rfft_fast_init_f32(&h->fft_instance, HYDRO_FFT_SIZE) != ARM_MATH_SUCCESS) {
		return HYDRO_ERROR;
	}

	/* Pre-compute Hanning window coefficients */
	arm_hanning_f32(h->window, HYDRO_FFT_SIZE);

	return HYDRO_OK;
}

/**
 * @brief  Start circular SPI DMA reception.
 *
 *         The DMA will fill dma_buffer continuously.  Half-transfer
 *         and transfer-complete interrupts trigger the callbacks that
 *         copy samples into the processing buffer.
 *
 * @param  h  Initialized Hydro_Handler_t
 * @return HYDRO_OK on success, HYDRO_ERROR if HAL returns an error
 */
Hydro_Status_t Hydro_StartReceive(Hydro_Handler_t *h)
{
	/* Assert CS (active low) */
	HAL_GPIO_WritePin(h->cs_port, h->cs_pin, GPIO_PIN_RESET);

	if (HAL_SPI_Receive_DMA(h->hspi,
	                        h->dma_buffer,
	                        HYDRO_DMA_BUF_SIZE) != HAL_OK) {
		HAL_GPIO_WritePin(h->cs_port, h->cs_pin, GPIO_PIN_SET);
		return HYDRO_ERROR;
	}
	return HYDRO_OK;
}

/**
 * @brief  Run the full DSP processing pipeline on the latest samples.
 *
 *         Pipeline:
 *           1. DC removal       — arm_mean_f32 + arm_offset_f32
 *           2. Windowing        — arm_mult_f32  (Hanning)
 *           3. FFT              — arm_rfft_fast_f32
 *           4. Magnitude        — arm_cmplx_mag_f32
 *           5. Peak detection   — arm_max_f32
 *
 * @param  h  Initialized Hydro_Handler_t
 * @return HYDRO_OK        Processing complete, results updated
 *         HYDRO_NO_DATA   No new samples available yet
 */
Hydro_Status_t Hydro_Process(Hydro_Handler_t *h)
{
	if (!h->data_ready) {
		return HYDRO_NO_DATA;
	}
	h->data_ready = false;

	/* ── 1. Remove DC offset ─────────────────────────────────────── */
	float32_t mean;
	arm_mean_f32(h->fft_input, HYDRO_FFT_SIZE, &mean);
	arm_offset_f32(h->fft_input, -mean, h->fft_input, HYDRO_FFT_SIZE);

	/* ── 2. Apply Hanning window ─────────────────────────────────── */
	arm_mult_f32(h->fft_input, h->window, h->fft_input, HYDRO_FFT_SIZE);

	/* ── 3. Compute FFT (real input → interleaved complex output) ─ */
	arm_rfft_fast_f32(&h->fft_instance, h->fft_input, h->fft_output, 0);

	/* ── 4. Compute magnitudes of each frequency bin ─────────────── */
	arm_cmplx_mag_f32(h->fft_output, h->magnitudes, HYDRO_FFT_SIZE / 2);

	/* ── 5. Find peak frequency (skip bin 0 = DC) ────────────────── */
	float32_t max_val;
	uint32_t  max_idx;
	arm_max_f32(&h->magnitudes[1], (HYDRO_FFT_SIZE / 2) - 1, &max_val, &max_idx);

	/* Convert bin index to Hz.  +1 because we skipped bin 0. */
	h->peak_magnitude   = max_val;
	h->dominant_freq_hz = (uint32_t)(((float32_t)(max_idx + 1)) * HYDRO_FREQ_RES_HZ);

	/* Simple threshold-based detection */
	h->signal_detected = (h->peak_magnitude > HYDRO_DETECT_THRESHOLD);

	return HYDRO_OK;
}

/* ── DMA Callbacks ─────────────────────────────────────────────────── */

/**
 * @brief  Called from HAL_SPI_RxHalfCpltCallback().
 *         First half of the DMA buffer is full.
 */
void Hydro_SPI_RxHalfCpltCallback(Hydro_Handler_t *h, SPI_HandleTypeDef *hspi)
{
	if (hspi == h->hspi) {
		Hydro_LoadSamples(h, HYDRO_BUF_FIRST_HALF);
	}
}

/**
 * @brief  Called from HAL_SPI_RxCpltCallback().
 *         Second half of the DMA buffer is full.
 */
void Hydro_SPI_RxCpltCallback(Hydro_Handler_t *h, SPI_HandleTypeDef *hspi)
{
	if (hspi == h->hspi) {
		Hydro_LoadSamples(h, HYDRO_BUF_SECOND_HALF);
		/* Deassert CS — full buffer received */
		HAL_GPIO_WritePin(h->cs_port, h->cs_pin, GPIO_PIN_SET);
	}
}
