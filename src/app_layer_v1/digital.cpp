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
#include "digital.h"
#include "protocol.h"
#include "logging.h"

void SetDigitalOutLevel(int pin, int value) {
  digitalWrite(pin, value ? HIGH : LOW);
}

/*
  The Teensy 4.0, which is based on the NXP i.MX RT1062
  microcontroller, doesn't have a direct equivalent to the "Change
  Notification" (CN) interrupts found in PIC
  microcontrollers. However, it does support external interrupts on
  its GPIO pins, which can be configured to trigger on rising,
  falling, or change events.

  You can use the attachInterrupt() function in the Arduino IDE (or the
  Teensyduino extension) to handle these interrupts. For detecting
  changes on a pin, you would typically configure an interrupt that
  triggers on both rising and falling edges to achieve similar
  functionality to CN interrupts.
  
  If you need to monitor multiple pins for changes, you would have to
  set up individual interrupts for each pin, as the Teensy does not have
  a built-in mechanism to monitor multiple pins with a single interrupt
  like CN interrupts do in PIC microcontrollers.

*/

// fixme: ~/github/ioio/firmware/app_layer_v1/digital.c

void SetChangeNotify(int pin, int changeNotify) {

  // NUM_DIGITAL_PINS
  //  void attachInterrupt(uint8_t pin, void (*function)(void), int mode);
// Remove a previously configured attachInterrupt() function from a pin.
//void detachInterrupt(uint8_t pin);

  
}

static void SendDigitalInStatusMessage(int pin, int value) {
  log("SendDigitalInStatusMessage(%d, %d)", pin, value);
  OUTGOING_MESSAGE msg;
  msg.type = REPORT_DIGITAL_IN_STATUS;
  msg.args.report_digital_in_status.pin = pin;
  msg.args.report_digital_in_status.level = value;
  AppProtocolSendMessage(&msg);
}

