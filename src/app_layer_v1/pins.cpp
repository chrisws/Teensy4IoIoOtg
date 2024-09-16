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

#include <assert.h>

#include "pins.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))
#define SFR volatile unsigned int

unsigned int CNENB = 0x0000;
unsigned int CNENC = 0x0000;
unsigned int CNEND = 0x0000;
unsigned int CNENE = 0x0000;
unsigned int CNENF = 0x0000;
unsigned int CNENG = 0x0000;
unsigned int CNFORCEB = 0x0000;
unsigned int CNFORCEC = 0x0000;
unsigned int CNFORCED = 0x0000;
unsigned int CNFORCEE = 0x0000;
unsigned int CNFORCEF = 0x0000;
unsigned int CNFORCEG = 0x0000;
unsigned int CNBACKUPB = 0x0000;
unsigned int CNBACKUPC = 0x0000;
unsigned int CNBACKUPD = 0x0000;
unsigned int CNBACKUPE = 0x0000;
unsigned int CNBACKUPF = 0x0000;
unsigned int CNBACKUPG = 0x0000;

typedef struct {
  SFR* tris;
  SFR* ansel;
  SFR* port;
  SFR* lat;
  SFR* odc;
  unsigned int* fake_cnen;
  unsigned int* cn_backup;
  unsigned int* cn_force;
  unsigned int pos_mask;
  unsigned int neg_mask;
  PORT name;
  int nbit;
} PORT_INFO;

#define MAKE_PORT_INFO(port, num) {             \
    &TRIS##port,                                \
      &ANS##port,                               \
      &PORT##port,                              \
      &LAT##port,                               \
      &ODC##port,                               \
      &CNEN##port,                              \
      &CNBACKUP##port,                          \
      &CNFORCE##port,                           \
      (1 << num),                               \
      ~(1 << num),                              \
      PORT_##port,                              \
      num                                       \
      }

typedef struct {
  SFR *cnen;
  SFR *cnpu;
  SFR *cnpd;
  unsigned int pos_mask;
  unsigned int neg_mask;
} CN_INFO;

void PinSetTris(int pin, int val) {
}

void PinSetAnsel(int pin, int val) {
}

void PinSetLat(int pin, int val) {
}

int PinGetPort(int pin) {
  return 0;
}
void PinSetOdc(int pin, int val) {
}

void PinSetCnen(int pin, int cnen) {
}

void PinSetCnforce(int pin) {
}

void PinSetCnpu(int pin, int cnpu) {
}

void PinSetCnpd(int pin, int cnpd) {
}

void PinSetRpor(int pin, int per) {
}

int PinFromPortB(int bit) {
  return 0; // port_to_pin[0][bit];
};

int PinFromPortC(int bit) {
  return 0; // port_to_pin[1][bit];
};

int PinFromPortD(int bit) {
  return 0; // port_to_pin[2][bit];
};

int PinFromPortE(int bit) {
  return 0; // port_to_pin[3][bit];
};

int PinFromPortF(int bit) {
  return 0; // port_to_pin[4][bit];
};

int PinFromPortG(int bit) {
  return 0; // port_to_pin[5][bit];
};

int PinFromAnalogChannel(int ch) {
  return 0; // analog_to_pin[ch];
}

int PinToAnalogChannel(int pin) {
  // if (pin < MIN_ANALOG_PIN
  //     || pin - MIN_ANALOG_PIN >= ARRAY_SIZE(pin_to_analog)) {
  //   return -1;
  // }
  return 0; // pin_to_analog[pin - MIN_ANALOG_PIN];
}

int PinToRpin(int pin) {
  return 0; // pin_to_rpin[pin];
}

void PinToPortBit(int pin, PORT *port, int *nbit) {
  // const PORT_INFO* info = &port_info[pin];
  // *port = info->name;
  // *nbit = info->nbit;
}

