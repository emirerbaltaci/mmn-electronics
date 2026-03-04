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

#ifndef INC_BAR30_CONFIG_H_
#define INC_BAR30_CONFIG_H_

#include "bar30.h"

/*
 * BAR30_SETUP_OSR              Oversampling Ratio
 * BAR30_OSR_256                (0.6ms)
 * BAR30_OSR_512                (1.17ms)
 * BAR30_OSR_1024               (2.28ms)
 * BAR30_OSR_2048               (4.54ms)
 * BAR30_OSR_4096               (9.04ms)
 * BAR30_OSR_8192               (18.08ms)
 */
#define BAR30_SETUP_OSR BAR30_OSR_8192

/*
 * BAR30_SETUP_FLUID_DENSITY    Fluid Density (kg/m^3)
 * 997.0f                       Freshwater
 * 1029.0f                      Seawater
 */
#define BAR30_SETUP_FLUID_DENSITY 1029.0f

#endif /* INC_BAR30_CONFIG_H_ */
