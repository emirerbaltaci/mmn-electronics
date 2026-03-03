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

#ifndef INC_EKF_CONFIG_H_
#define INC_EKF_CONFIG_H_

#define EKF_IMU_LEVER_ARM_X 0.1f
#define EKF_IMU_LEVER_ARM_Y 0.0f
#define EKF_IMU_LEVER_ARM_Z 0.05f

#define EKF_MAG_PRESCALER 10

#define EKF_GRAVITY_NORM_MIN 9.0f
#define EKF_GRAVITY_NORM_MAX 10.6f

#define EKF_R_ACCEL 0.05f
#define EKF_R_MAG 0.1f
#define EKF_R_BARO 0.0025f
#define EKF_R_ZUPT 0.0001f

#define EKF_P_INIT_POS 0.1f
#define EKF_P_INIT_VEL 0.1f
#define EKF_P_INIT_ATT 0.01f
#define EKF_P_INIT_BG 0.001f
#define EKF_P_INIT_BA 0.001f

#define EKF_Q_VEL_NOISE 4.712e-7f
#define EKF_Q_ATT_NOISE 2.388e-9f
#define EKF_Q_BG_NOISE 1.0e-7f
#define EKF_Q_BA_NOISE 1.0e-7f

#define EKF_EARTH_MAG_X 0.549f
#define EKF_EARTH_MAG_Y 0.054f
#define EKF_EARTH_MAG_Z 0.834f

#define EKF_CHI_SQUARE_TH_DOF1 6.635f
#define EKF_CHI_SQUARE_TH_DOF2 9.210f
#define EKF_CHI_SQUARE_TH_DOF3 11.345f
#define EKF_CHI_SQUARE_TH_DOF6 16.812f

#endif /* INC_EKF_CONFIG_H_ */
