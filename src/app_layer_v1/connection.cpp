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

#include <USBHost_t36.h>

#include "connection.h"
#include "usb_android.h"

#define CHANNEL_HANDLE_ACCESSORY 1
#define CHANNEL_HANDLE_CDC 2

uint32_t baud = 1000000;
uint32_t format = USBHOST_SERIAL_8N1;

USBHost usbHost;
USBHostAndroid android(usbHost);
USBSerial usbSerial(usbHost);

CHANNEL_HANDLE openChannelHandle = 0;
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
  if (android.connected()) {
    if (android.isAccessoryMode()) {
      android.readWrite(callback);
    } else {
      android.beginAccessory();
    }
  }
}

void ConnectionInit() {
  usbHost.begin();
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
  case CHANNEL_HANDLE_ACCESSORY:
    result = android.connected();
    break;
  case CHANNEL_HANDLE_CDC:
    result = usbSerial.available();
    break;
  default:
    result = false;
    break;
  }
  return result;
}

CHANNEL_HANDLE ConnectionOpenChannelAccessory(ChannelReceiveCallback cb) {
  openChannelHandle = android.isAccessoryMode() ? CHANNEL_HANDLE_ACCESSORY : 0;
  callback = cb;
  return openChannelHandle;
}

CHANNEL_HANDLE ConnectionOpenChannelCdc(ChannelReceiveCallback cb) {
  openChannelHandle = usbSerial.available() ? CHANNEL_HANDLE_CDC : 0;
  callback = cb;
  return openChannelHandle;
}

void ConnectionSend(CHANNEL_HANDLE ch, const uint8_t *data, int size) {
  switch (ch) {
  case CHANNEL_HANDLE_ACCESSORY:
    if (android.isAccessoryMode()) {
      android.write(data, size);
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
    result = android.isAccessoryMode();
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
  if (ch == CHANNEL_HANDLE_ACCESSORY && android.isAccessoryMode()) {
    android.end();
  }
  openChannelHandle = 0;
}

int ConnectionGetMaxPacket(CHANNEL_HANDLE ch) {
  int result;
  switch (ch) {
  case CHANNEL_HANDLE_ACCESSORY:
    result = android.maxPacketSize();
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
  android.end();
  usbSerial.end();
}
