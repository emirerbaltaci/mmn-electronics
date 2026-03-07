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

#ifndef VEHICLE_INC_AUVFLAGS_H_
#define VEHICLE_INC_AUVFLAGS_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	AUV_INIT = 0,
	AUV_DISARMED = 1,
	AUV_PREARMED = 2,
	AUV_ARMED = 3,
	AUV_MISSION = 4,
	AUV_DISARMED_MANUAL = 11,
	AUV_PREARMED_MANUAL = 12,
	AUV_ARMED_MANUAL = 13,
	AUV_DISARMED_ERR = 21,
	AUV_PREARMED_ERR = 22,
	AUV_ARMED_ERR = 23,
	AUV_DISARMED_MANUAL_ERR = 31,
	AUV_PREARMED_MANUAL_ERR = 32,
	AUV_ARMED_MANUAL_ERR = 33,
	MCU_HARDFAULT = 255
} AUV_VehicleState_t;

typedef enum {
	AUV_TASK_DEAD,
	AUV_TASK_ALIVE
} AUV_TaskState_t;

extern volatile uint32_t configurableFlags;
extern uint8_t ncom_flag_bit_meanings[32];
extern uint32_t ncom_flag_bit_interrupts_mask;

void AUV_Flags_Update(void);

#endif /* VEHICLE_INC_AUVFLAGS_H_ */
