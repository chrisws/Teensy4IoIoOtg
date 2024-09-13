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

#include "Compiler.h"
#include "libconn/connection.h"
#include "features.h"
#include "protocol.h"
#include "logging.h"

// define in non-const arrays to ensure data space
static char descManufacturer[] = "IOIO Open Source Project";
static char descModel[] = "IOIO";
static char descDesc[] = "IOIO Standard Application";
static char descVersion[] = FW_IMPL_VER;
static char descUri[] = "https://github.com/ytai/ioio/wiki/ADK";
static char descSerial[] = "N/A";

const char* accessoryDescs[6] = {
  descManufacturer,
  descModel,
  descDesc,
  descVersion,
  descUri,
  descSerial
};

typedef enum {
  STATE_INIT,
  STATE_OPEN_CHANNEL,
  STATE_WAIT_CHANNEL_OPEN,
  STATE_CONNECTED,
  STATE_ERROR
} STATE;

static STATE state = STATE_INIT;
static CHANNEL_HANDLE handle;

void AppCallback(const void* data, UINT32 data_len, int_or_ptr_t arg);

static inline CHANNEL_HANDLE OpenAvailableChannel() {
  int_or_ptr_t arg = { .i = 0 };
  if (ConnectionTypeSupported(CHANNEL_TYPE_ADB)) {
    if (ConnectionCanOpenChannel(CHANNEL_TYPE_ADB)) {
      return ConnectionOpenChannelAdb("tcp:4545", &AppCallback, arg);
    }
  } else if (ConnectionTypeSupported(CHANNEL_TYPE_ACC)) {
    if (ConnectionCanOpenChannel(CHANNEL_TYPE_ACC)) {
      return ConnectionOpenChannelAccessory(&AppCallback, arg);
    }
  } else if (ConnectionTypeSupported(CHANNEL_TYPE_BT)) {
    if (ConnectionCanOpenChannel(CHANNEL_TYPE_BT)) {
      return ConnectionOpenChannelBtServer(&AppCallback, arg);
    }
  } else if (ConnectionTypeSupported(CHANNEL_TYPE_CDC_DEVICE)) {
    if (ConnectionCanOpenChannel(CHANNEL_TYPE_CDC_DEVICE)) {
      return ConnectionOpenChannelCdc(&AppCallback, arg);
    }
  }
  return INVALID_CHANNEL_HANDLE;
}

void AppCallback(const void* data, UINT32 data_len, int_or_ptr_t arg) {
  if (data) {
    if (!AppProtocolHandleIncoming(data, data_len)) {
      // got corrupt input. need to close the connection and soft reset.
      log_printf("Protocol error");
      state = STATE_ERROR;
    }
  } else {
    // connection closed, soft reset and re-establish
    if (state == STATE_CONNECTED) {
      log_printf("Channel closed");
      SoftReset();
    } else {
      log_printf("Channel failed to open");
    }
    state = STATE_OPEN_CHANNEL;
  }
}

int main() {
  log_init();
  log_printf("***** Hello from app-layer! *******");

  SoftReset();
  ConnectionInit();
  while (1) {
    ConnectionTasks();
    switch (state) {
    case STATE_INIT:
      handle = INVALID_CHANNEL_HANDLE;
      state = STATE_OPEN_CHANNEL;
      break;

    case STATE_OPEN_CHANNEL:
      if ((handle = OpenAvailableChannel()) != INVALID_CHANNEL_HANDLE) {
        log_printf("Connected");
        state = STATE_WAIT_CHANNEL_OPEN;
      }
      break;

    case STATE_WAIT_CHANNEL_OPEN:
      if (ConnectionCanSend(handle)) {
        log_printf("Channel open");
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
  return 0;
}
