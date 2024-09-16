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

// Here's the deal:
// We want to measure the time between two edges and report it. We do not want
// our reports to happen more often than 200 times a second.
// We set timer 5 to fire an interrupt 200 times a second and from within this
// interrupt we start a capture on any modules that should be enabled and are
// not currently capturing.
//
// When measuring pulses (rising-to-falling or falling-to-rising):
// Once the first edge is detected, an incap interrupt will fire. We set this
// interrupt priority to 6, so that we can quickly prepare to the next edge
// before we miss it. Then we can reduce the priority back to 1. When the next
// interrupt comes, we have two time values in the incap FIFO, which we can
// subtract to obtain the desired delta, and we send a report to the client.
// Then, we can turn off the module and re-arm it to be triggered on the next
// timer interrupt.
//
// When measuring period (rising-to-rising):
// This is simpler: we set the module to fire after two captures and we set the
// interrupt priority directly to 1. Then we handle the interrupt the same as
// we handle the trailing edge of the first case.

#include <cstdint>

#include "incap.h"
#include "platform.h"
#include "logging.h"

#define LEADING_PRIORITY 6
#define TRAILING_PRIORITY 1

typedef struct {
  volatile unsigned int con1;
  volatile unsigned int con2;
  volatile unsigned int buf;
  volatile unsigned int tmr;
} INCAP_REG;

typedef enum {
  LEADING = 0,
  TRAILING = 1
} EDGE_STATE;

// Records for each module whether the next interrupt is a leading edge or a
// traling edge.
static EDGE_STATE edge_states[NUM_INCAP_MODULES];

// For each module: the value that needs to be written to con1 in order to
// enable the module in the right mode.
static uint16_t con1_vals[NUM_INCAP_MODULES];

// For each module, if true, edge type will be flipped on every interrupt.
// This is used for measuring pulses, as opposed to period.
static unsigned flip[NUM_INCAP_MODULES];

// A bit mask, in which if bit k is set, module number k needs to be turned on
// upon the next timer interrupt.
static unsigned armed = 0;

static void InCapConfigInternal(int incap_num, int double_prec, int mode, int clock, int external);

void InCapInit() {
}

static inline void InCapArm(int incap_num, int double_prec) {
}

static inline void InCapDisarm(int incap_num, int double_prec) {
}

static void InCapConfigInternal(int incap_num, int double_prec, int mode, int clock, int external) {
}

void InCapConfig(int incap_num, int double_prec, int mode, int clock) {
  log_printf("InCapConfig(%d, %d, %d, %d)", incap_num, double_prec, mode, clock);
  InCapConfigInternal(incap_num, double_prec, mode, clock, 1);
}

inline static void ReportCapture(int incap_num, int double_prec) {
}

static void ICInterrupt(int incap_num) {
}
