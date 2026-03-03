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

#ifndef INC_TASK_CONFIG_H_
#define INC_TASK_CONFIG_H_

#define TASK_STATEESTIMATE_SLEEP_MS 1
#define TASK_CONTROL_SLEEP_MS 5
#define TASK_NCOM_SLEEP_MS 20
#define TASK_NCOM_PACKET_TIMEOUT_MS 50
#define TASK_NCOM_HANDSHAKE_TIMEOUT_MS 500
#define TASK_SENSOR_SLEEP_MS 100
#define TASK_SYSMONITOR_SLEEP_MS 50

/**
 * Application Timing Constants
 */
#define TASK_CONTROL_PID_DT 0.005f

#endif /* INC_TASK_CONFIG_H_ */
