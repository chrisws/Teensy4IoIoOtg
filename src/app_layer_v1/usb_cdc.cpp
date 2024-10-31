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
#include "connection.h"
#include "logging.h"

ChannelReceiveCallback _callback = nullptr;
char _readBuffer[CDC_RX_SIZE_480];

void ConnectionInit() {
  delay(1000);
  // should match SerialPortIOIOConnection.java
  Serial.begin(115200);
  while (!Serial);
}

void ConnectionTasks() {
  if (Serial.available()) {
    int bytesRead = Serial.readBytes(_readBuffer, sizeof(_readBuffer));
    if (bytesRead > 0 && _callback) {
      _callback((uint8_t *)_readBuffer, bytesRead);
    }
  }
}

bool ConnectionTypeSupported(CHANNEL_TYPE con) {
  return con == CHANNEL_TYPE::CHANNEL_TYPE_CDC;
}

bool ConnectionCanOpenChannel(CHANNEL_TYPE con) {
  return con == CHANNEL_TYPE::CHANNEL_TYPE_CDC;
}

CHANNEL_HANDLE ConnectionOpenChannel(CHANNEL_TYPE con, ChannelReceiveCallback cb) {
  CHANNEL_HANDLE result;
  if (con == CHANNEL_TYPE::CHANNEL_TYPE_CDC) {
    _callback = cb;
    result = CHANNEL_HANDLE_CDC;
  } else {
    result = INVALID_CHANNEL_HANDLE;
  }
  return result;
}

void ConnectionSend(CHANNEL_HANDLE ch, const uint8_t *data, int size) {
  if (ch == CHANNEL_HANDLE_CDC) {
    Serial.write(data, size);
  }
}

int ConnectionCanSend(CHANNEL_HANDLE ch) {
  int result;
  if (ch == CHANNEL_HANDLE_CDC) {
    result = Serial.availableForWrite();
  } else {
    result = 0;
  }
  return result;
}

void ConnectionCloseChannel(CHANNEL_HANDLE ch) {
}

void ConnectionShutdownAll() {
  Serial.end();
}

