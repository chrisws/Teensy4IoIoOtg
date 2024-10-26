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

#include "usb_host.h"
#include "usb_android.h"
#include "connection.h"
#include "logging.h"

#if !defined(USB_TEST)

#define CHANNEL_HANDLE_ACCESSORY 1
#define CHANNEL_HANDLE_CDC 2

// baud rate setting is virtual when communicating over USB.
uint32_t baud = 115200;

USBHost usbHost;
USBAndroid usbAndroid(usbHost);
USBSerial_BigBuffer usbSerial(usbHost, 1);

CHANNEL_HANDLE openChannelHandle = INVALID_CHANNEL_HANDLE;
ChannelReceiveCallback callback = nullptr;

// Define the buffer to use to copy between the two devices
uint8_t cdcBuffer[CDC_RX_SIZE_480];

void cdcTask() {
  if (usbSerial.available()) {
    int bytesRead = usbSerial.readBytes(cdcBuffer, sizeof(cdcBuffer));
    if (bytesRead > 0 && callback) {
      // Process the incoming data
      callback(cdcBuffer, bytesRead);
    }
  }
}

void accessoryTask() {
  if (usbAndroid.connected()) {
    if (usbAndroid.isAccessoryMode()) {
      usbAndroid.readWrite(callback);
    } else {
      usbAndroid.beginAccessory();
    }
  }
}

void ConnectionInit() {
  usbHost.begin();
  delay(2000);
  usbSerial.begin(baud);
}

void ConnectionTasks() {
  // Poll the USB host to handle connected devices
  usbHost.Task();

  switch (openChannelHandle) {
  case CHANNEL_HANDLE_ACCESSORY:
    accessoryTask();
    break;
  case CHANNEL_HANDLE_CDC:
    cdcTask();
    break;
  }
}

bool ConnectionTypeSupported(CHANNEL_TYPE con) {
  return (con == CHANNEL_TYPE::CHANNEL_TYPE_ACC || con == CHANNEL_TYPE::CHANNEL_TYPE_CDC_DEVICE);
}

bool ConnectionCanOpenChannel(CHANNEL_TYPE con) {
  bool result;
  switch (con) {
  case CHANNEL_TYPE::CHANNEL_TYPE_ACC:
    result = usbAndroid.connected();
    break;
  case CHANNEL_TYPE::CHANNEL_TYPE_CDC_DEVICE:
    result = true;
    break;
  default:
    result = false;
    break;
  }
  return result;
}

CHANNEL_HANDLE ConnectionOpenChannelAccessory(ChannelReceiveCallback cb) {
  callback = cb;
  openChannelHandle = usbAndroid.isAccessoryMode() ? CHANNEL_HANDLE_ACCESSORY : INVALID_CHANNEL_HANDLE;
  return openChannelHandle;
}

CHANNEL_HANDLE ConnectionOpenChannelCdc(ChannelReceiveCallback cb) {
  callback = cb;
  openChannelHandle = CHANNEL_HANDLE_CDC;
  return openChannelHandle;
}

void ConnectionSend(CHANNEL_HANDLE ch, const uint8_t *data, int size) {
  switch (ch) {
  case CHANNEL_HANDLE_ACCESSORY:
    if (usbAndroid.isAccessoryMode()) {
      usbAndroid.write(data, size);
    }
    break;
  case CHANNEL_HANDLE_CDC:
    usbSerial.write(data, size);
    break;
  }
}

bool ConnectionCanSend(CHANNEL_HANDLE ch) {
  bool result;
  switch (ch) {
  case CHANNEL_HANDLE_ACCESSORY:
    result = usbAndroid.isAccessoryMode();
    break;
  case CHANNEL_HANDLE_CDC:
    result = usbSerial.availableForWrite() > 0;
    break;
  default:
    result = false;
  }
  return result;
}

void ConnectionCloseChannel(CHANNEL_HANDLE ch) {
  if (ch == CHANNEL_HANDLE_ACCESSORY && usbAndroid.isAccessoryMode()) {
    usbAndroid.end();
  }
  openChannelHandle = INVALID_CHANNEL_HANDLE;
}

int ConnectionGetMaxPacket(CHANNEL_HANDLE ch) {
  int result;
  switch (ch) {
  case CHANNEL_HANDLE_ACCESSORY:
    result = usbAndroid.maxPacketSize();
    break;
  case CHANNEL_HANDLE_CDC:
    // high-speed USB
    result = CDC_RX_SIZE_480;
    break;
  default:
    result = 0;
  }
  return result;
}

void ConnectionShutdownAll() {
  usbAndroid.end();
  usbSerial.end();
}

#else

USBHost usbHost;
USBSerial usbSerial(usbHost);

void __setup() {
  log_init();
  usbHost.begin();
  delay(2000);

  if (usbSerial) {
    usbSerial.begin(baud);
    while (!usbSerial && millis() < 5000) {}
    usbSerial.println("USB Serial initialized!");
  }
}

void __loop() {
  usbHost.Task();
  blink(1000);
  if (usbSerial && usbSerial.availableForWrite()) {
    usbSerial.println("Data from Teensy USBSerial.");
  } else {
    Serial.println("USB Serial not available for write.");
  }
  delay(1000);
}

#endif
