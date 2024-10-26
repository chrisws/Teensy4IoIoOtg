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

#include <cstring>
#include <Arduino.h>

#include "pins.h"
#include "logging.h"
#include "protocol.h"
#include "adc.h"
#include "pwm.h"
#include "uart.h"
#include "spi.h"
#include "i2c.h"
#include "timers.h"
#include "sequencer.h"
#include "incap.h"
#include "sync.h"

void SetPinDigitalOut(int pin, int value, int open_drain) {
  log("SetPinDigitalOut(%d, %d, %d)", pin, value, open_drain);

}

void SetPinDigitalIn(int pin, int pull) {
  log("SetPinDigitalIn(%d, %d)", pin, pull);

}

void SetPinAnalogIn(int pin) {
  log("SetPinAnalogIn(%d)", pin);

}

void SetPinCapSense(int pin) {
  log("SetPinCapSense(%d)", pin);

}

void SetPinPwm(int pin, int pwm_num, int enable) {
  log("SetPinPwm(%d, %d)", pin, pwm_num);


}

void SetPinUart(int pin, int uart_num, int dir, int enable) {
  log("SetPinUart(%d, %d, %d, %d)", pin, uart_num, dir, enable);

}

void SetPinSpi(int pin, int spi_num, int mode, int enable) {
  log("SetPinSpi(%d, %d, %d, %d)", pin, spi_num, mode, enable);


}

void SetPinInCap(int pin, int incap_num, int enable) {
  log("SetPinInCap(%d, %d, %d)", pin, incap_num, enable);
}

static void PinsInit() {
  // LED pin: output, open-drain, high (off)
  SetPinDigitalOut(0, 1, 1);
  for (int i = 1; i < NUM_PINS; ++i) {
    // all other pins: input, no-pull
    SetPinDigitalIn(i, 0);
  }
}

void HardReset() {
  log("HardReset()");
  log("Rebooting...");

  // Write to the AIRCR register to trigger reset
  SCB_AIRCR = 0x05FA0004;
}

void SoftReset() {
  PRIORITY(7) {
    log("SoftReset()");
    TimersInit();
    PinsInit();
    PWMInit();
    ADCInit();
    UARTInit();
    SPIInit();
    I2CInit();
    InCapInit();
    SequencerInit();
  }
}

void CheckInterface(uint8_t *interface_id) {
  OUTGOING_MESSAGE msg;
  msg.type = CHECK_INTERFACE_RESPONSE;
  msg.args.check_interface_response.supported
    = (memcmp(interface_id, PROTOCOL_IID_IOIO0005, 8) == 0)
    || (memcmp(interface_id, PROTOCOL_IID_IOIO0004, 8) == 0)
    || (memcmp(interface_id, PROTOCOL_IID_IOIO0003, 8) == 0)
    || (memcmp(interface_id, PROTOCOL_IID_IOIO0002, 8) == 0)
    || (memcmp(interface_id, PROTOCOL_IID_IOIO0001, 8) == 0);
  AppProtocolSendMessage(&msg);
}
