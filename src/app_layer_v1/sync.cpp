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

#include <Arduino.h>
#include <imxrt.h>

#include "sync.h"

uint8_t g_priority[NVIC_NUM_INTERRUPTS];

// PIC: priorities 1 - 7
// Cortex-M7: 0,16,32,48,64,80,96,112,128,144,160,176,192,208,224,240
// not sure how to marry these concepts, note: cannot use broken teensy4/core_cm7.h 

void SyncInit() {
  for (int i = 0; i < NVIC_NUM_INTERRUPTS; i++) {
    g_priority[i] = NVIC_GET_PRIORITY(i);
  }
}

DisableInterrupts::DisableInterrupts(unsigned priority) : _priority(priority), _once(true) {
  __disable_irq();
  // for (int i = 0; i < NVIC_NUM_INTERRUPTS; i++) {
  //   if (g_priority[i] <= _priority) {
  //     //NVIC_DisableIRQ(i);
  //   }
  // }
}

bool DisableInterrupts::once() {
  bool result = _once;
  _once = false;
  return result;
}

DisableInterrupts::~DisableInterrupts() {
  __enable_irq();
  // for (int i = 0; i < NVIC_NUM_INTERRUPTS; i++) {
  //   if (g_priority[i] <= _priority) {
  //     // NVIC_EnableIRQ(i);
  //   }
  // }
}
