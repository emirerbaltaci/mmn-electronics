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

#ifndef NCOM_HANDLERS_H
#define NCOM_HANDLERS_H

#include <stdbool.h>
#include <string.h>
#include "ncom_rx.h"


bool NCOM_Handlers_Selector(NCOM_RX_t *rx);

bool NCOM_Handlers_Command(NCOM_RX_t *rx);
bool NCOM_Handlers_ConfigReq(NCOM_RX_t *rx);
bool NCOM_Handlers_ConfigSetMcu(NCOM_RX_t *rx);
bool NCOM_Handlers_ConfigSetFreertos(NCOM_RX_t *rx);
bool NCOM_Handlers_ConfigSetSensor(NCOM_RX_t *rx);
bool NCOM_Handlers_ConfigSetActuator(NCOM_RX_t *rx);
bool NCOM_Handlers_ConfigSetPID(NCOM_RX_t *rx);
bool NCOM_Handlers_ConfigSetEKF(NCOM_RX_t *rx);
bool NCOM_Handlers_ConfigSetStartup(NCOM_RX_t *rx);

#endif // NCOM_HANDLERS_H
