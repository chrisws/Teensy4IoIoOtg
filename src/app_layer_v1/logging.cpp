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

#ifdef ENABLE_LOGGING

#include <Arduino.h>
#include "logging.h"
#include "connection.h"
#include "protocol.h"

#include <cstdio>
#include <cstring>
#include <stdarg.h>

#define MAX_BUFFER_SIZE 1024
uint8_t _buffer[MAX_BUFFER_SIZE];
long _lastMillis = 0;
bool _ledOn = true;

void _blink(uint32_t rate) {
  if (millis() - _lastMillis > (_ledOn ? 100 : rate)) {
    _ledOn = !_ledOn;
    _lastMillis = millis();
    digitalWriteFast(LED_BUILTIN, _ledOn ? HIGH : LOW);
  }
}

void log_init() {
  // pinMode(LED_BUILTIN, OUTPUT);
}

void _log(const char *format, ...) {
  va_list args;
  va_start(args, format);
  unsigned size = vsnprintf(NULL, 0, format, args);
  va_end(args);

  if (size < MAX_BUFFER_SIZE) {
    _buffer[0] = '\0';
    va_start(args, format);
    vsnprintf(_buffer, size + 1, format, args);
    va_end(args);
    _buffer[size] = '\0';
    AppProtocolSendLogging(_buffer, size);
  }
}

#endif  // ENABLE_LOGGING
