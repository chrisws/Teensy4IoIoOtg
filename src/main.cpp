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

#include "app_layer_v1/connection.h"
#include "app_layer_v1/features.h"
#include "app_layer_v1/protocol.h"
#include "app_layer_v1/logging.h"
#include "app_layer_v1/sync.h"

typedef enum {
  STATE_INIT,
  STATE_OPEN_CHANNEL,
  STATE_WAIT_CHANNEL_OPEN,
  STATE_CONNECTED,
  STATE_ERROR
} STATE;

static STATE state = STATE_INIT;
static CHANNEL_HANDLE handle;

void AppCallback(const uint8_t *data, uint32_t data_len) {
  if (data) {
    if (!AppProtocolHandleIncoming(data, data_len)) {
      // got corrupt input. need to close the connection and soft reset.
      log("Protocol error");
      state = STATE_ERROR;
    }
  } else {
    // connection closed, soft reset and re-establish
    if (state == STATE_CONNECTED) {
      log("Channel closed");
      SoftReset();
    } else {
      log("Channel failed to open");
    }
    state = STATE_OPEN_CHANNEL;
  }
}

static inline CHANNEL_HANDLE OpenAvailableChannel() {
  CHANNEL_HANDLE result;
  if (ConnectionTypeSupported(CHANNEL_TYPE_ACC) && ConnectionCanOpenChannel(CHANNEL_TYPE_ACC)) {
    result = ConnectionOpenChannel(CHANNEL_TYPE_ACC, &AppCallback);
  } else {
    result = ConnectionOpenChannel(CHANNEL_TYPE_CDC, &AppCallback);
  }
  return result;
}

extern "C" void setup() {
  log_init();
  SyncInit();
  SoftReset();
  ConnectionInit();
}

extern "C" void loop() {
  ConnectionTasks();
  switch (state) {
  case STATE_INIT:
    handle = INVALID_CHANNEL_HANDLE;
    state = STATE_OPEN_CHANNEL;
    break;

  case STATE_OPEN_CHANNEL:
    handle = OpenAvailableChannel();
    if (handle != INVALID_CHANNEL_HANDLE) {
      state = STATE_WAIT_CHANNEL_OPEN;
    }
    break;

  case STATE_WAIT_CHANNEL_OPEN:
    if (ConnectionCanSend(handle)) {
      log("Channel open");
      AppProtocolInit(handle);
      state = STATE_CONNECTED;
    }
    break;

  case STATE_CONNECTED:
    AppProtocolTasks(handle);
    break;

  case STATE_ERROR:
    ConnectionCloseChannel(handle);
    SoftReset();
    state = STATE_INIT;
    break;
  }
}
