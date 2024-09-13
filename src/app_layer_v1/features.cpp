/*
 * Original code by Ytai Ben-Tsvi, part of the IOIO-OTG project
 * https://github.com/ytai/ioio
 *
 * Modified by Chris Warren-Smith
 * These modifications were made to port the IOIO-OTG firmware to Teensy hardware.
 *
 * License: [Apache License, Version 2.0]
 * See the LICENSE file for full details.
 *
 * Portions of this code may have been rewritten or replaced entirely.
 */

#include "features.h"

void SetPinDigitalOut(int pin, int value, int open_drain) {
}

void SetPinDigitalIn(int pin, int pull) {
}

void SetPinAnalogIn(int pin) {
}

void SetPinCapSense(int pin) {
}

void SetPinPwm(int pin, int pwm_num, int enable) {
}

void SetPinUart(int pin, int uart_num, int dir, int enable) {
}

void SetPinSpi(int pin, int spi_num, int mode, int enable) {
}

void SetPinInCap(int pin, int incap_num, int enable) {
}

void HardReset() {
}

void SoftReset() {
}

void CheckInterface(uint8_t interface_id[8]) {
}

