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

#include "connection.h"

//
// CDC (Communications Device Class) is typically used for desktop communication.
// It allows devices like the Teensy to communicate with a desktop or laptop via USB
// as if it were a serial port (USB serial communication).
//
// Accessory mode is specifically for Android devices. In this mode, the Android device
// acts as a USB device, and the Teensy (or another host) communicates with it by acting
// as a USB host. Android Open Accessory (AOA) mode is commonly used for this purpose,
// where an Android device communicates with peripherals like the IOIO.
//

#include <USBHost_t36.h>
#include <usb_serial.h>

USBHost usbHost;
USBHostHub hub1(usbHost);
USBHostHub hub2(usbHost);
USBHostAndroid android(usbHost);
USBSerial usbSerial(usbHost);

const char *manufacturer = "YourCompany";
const char *model = "YourModel";
const char *description = "Description of Accessory";
const char *version = "1.0";
const char *uri = "http://yourcompany.com";
const char *serial = "00000001";

#define CHANNEL_HANDLE_ACCESSORY 1
#define CHANNEL_HANDLE_CDC 2
CHANNEL_HANDLE openChannelHandle = 0;

void handleCdc() {
  if (usbSerial.available()) {
    // Example: Check if there's data available from the PC
    uint8_t incomingData[64];  // Buffer for receiving data
    int bytesRead = usbSerial.read(incomingData, sizeof(incomingData));

    if (bytesRead > 0) {
      // Process the incoming data
      handleReceivedData(incomingData, bytesRead);
    }

    // Example: Send data to the PC if needed
    const char *message = "Hello from Teensy via CDC!";
    if (ConnectionCanSend(2)) {  // Channel 2 represents CDC
      ConnectionSend(2, (uint8_t*)message, strlen(message));
    }
  }
}

void handleAccessory() {
  if (android.connected()) {
    if (android.isAccessoryMode()) {
      // Example: Check if there's data available from Android
      uint8_t incomingData[64];  // Buffer for receiving data
      int bytesRead = android.read(incomingData, sizeof(incomingData));

      if (bytesRead > 0) {
        // Process the incoming data
        handleReceivedData(incomingData, bytesRead);
      }

      // Example: Send data to Android device if needed
      const char *message = "Hello from Teensy!";
      if (ConnectionCanSend(1)) {
        ConnectionSend(1, (uint8_t*)message, strlen(message));
      }

    } else {
      // Not in accessory mode, initiate AOA
      android.beginAccessory(manufacturer, model, description, version, uri, serial);
    }
  }
}

void ConnectionInit() {
  usbHost.begin();
}

void ConnectionTasks() {
  // Poll the USB host to handle connected devices
  usbHost.Task();

  switch (openChannelHandle) {
  case CHANNEL_HANDLE_ACCESSORY:
    handleAccessory();
    break;
  case CHANNEL_HANDLE_CDC:
    handleCdc();
    break;
  }
}

bool ConnectionTypeSupported(CHANNEL_TYPE con) {
  return (con == CHANNEL_TYPE::ACCESSORY || con == CHANNEL_TYPE::CDC);
}

bool ConnectionCanOpenChannel(CHANNEL_TYPE con) {
  bool result;
  switch (con) {
  case CHANNEL_TYPE::CDC:
    result = usbSerial.connected();
    break;
  case CHANNEL_HANDLE_CDC:
    result = android.connected();
    break;
  default:
    result = false;
    break;
  }
  return result;
}

CHANNEL_HANDLE ConnectionOpenChannelAccessory(ChannelCallback cb, intptr_t cb_arg) {
  openChannelHandle = android.isAccessoryMode() ? CHANNEL_HANDLE_ACCESSORY : 0;
  return openChannelHandle;
}

CHANNEL_HANDLE ConnectionOpenChannelCdc(ChannelCallback cb, intptr_t cb_arg) {
  openChannelHandle = usbSerial.available() ? CHANNEL_HANDLE_CDC : 0;
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
    // // Accessory mode typically supports 64-byte packets
    result = 64;
    break;
  case CHANNEL_HANDLE_CDC:
    // CDC packet size (typically 64 bytes for USB CDC)
    result = 64;
    break;
  default:
    result = 0;
  }
  return result;
}

void ConnectionShutdownAll() {
  android.end();
  usbHost.end();
}
