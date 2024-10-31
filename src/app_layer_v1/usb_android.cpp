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
#include "protocol.h"

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

USBAndroid::USBAndroid() {
}

bool USBAndroid::connected() {
  return false;
}

bool USBAndroid::isAccessoryMode() {
  return false;
}

int USBAndroid::maxPacketSize() {
  return 64;
}

int USBAndroid::readWrite(ChannelReceiveCallback callback) {
  return 0;
}

int USBAndroid::write(const uint8_t *data, int size) {
  return 0;
}

void USBAndroid::beginAccessory() {
}

void USBAndroid::end() {
}

// void accessoryTask() {
//   if (usbAndroid.connected()) {
//     if (usbAndroid.isAccessoryMode()) {
//       usbAndroid.readWrite(callback);
//     } else {
//       usbAndroid.beginAccessory();
//     }
//   }
// }

