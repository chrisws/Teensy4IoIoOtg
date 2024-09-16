/*
 * Teensy4 IOIO-OTG Project
 *
 * Copyright 2011 Ytai Ben-Tsvi. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL ARSHAN POURSOHI OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are those of the
 * authors and should not be interpreted as representing official policies, either expressed
 * or implied.
 *
 */

// Capabilities and features of specific platforms.
// A platform is a combination of a hardaware interface and bootloader ABI.
//
// Provides:
// NUM_PINS         - The number of physical pins on the board, including the
//                    on-board LED.
// NUM_PWM_MODULES  - The number of available PWM modules.
// NUM_UART_MODULES - The number of available UART modules.
// NUM_SPI_MODULES  - The number of available SPI modules.
// NUM_I2C_MODULES  - The number of available I2C modules.

#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#if MCU == IMXRT1062

#define NUM_PINS 40              // Teensy 4.0 has 40 digital pins
#define NUM_PWM_MODULES 12       // 12 PWM channels are available
#define NUM_UART_MODULES 8       // 8 UART modules (Serial1 to Serial8)
#define NUM_SPI_MODULES 3        // 3 SPI modules (SPI, SPI1, SPI2)
#define NUM_I2C_MODULES 3        // 3 I2C modules (Wire, Wire1, Wire2)
#define NUM_INCAP_MODULES 4      // 4 Input Capture (InCap) modules (FTM timers with capture)

typedef enum {
  PORT_B,
  PORT_C,
  PORT_D,
  PORT_E,
  PORT_F,
  PORT_G,
  NUM_PORTS
} PORT;

#else
  #error Unknown hardware
#endif

#endif  // __PLATFORM_H__
