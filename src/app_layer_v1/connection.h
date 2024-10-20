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

// This module provides functions for ongoing operation of the connection.
// ConnectionInit() must be called once to reset the state.
// ConnectionTasks() must be called periodically in order to provide context
// for all connection services.

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

#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <cstdint>

typedef int CHANNEL_HANDLE;

#define INVALID_CHANNEL_HANDLE (-1)

typedef enum {
  CHANNEL_TYPE_ACC,
  CHANNEL_TYPE_CDC_DEVICE,
} CHANNEL_TYPE;

// data != NULL -> incoming data
// data = NULL, size = 0 -> Closed normally
// data = NULL, size = 1 -> Closed as result of error
//
// arg is whichever value the client provided when opening the channel.
typedef void (*ChannelReceiveCallback) (const uint8_t *data, uint32_t size);

// Reset the state of all connection modules.
void ConnectionInit();

// Needs to be called by the application periodically in order to provide
// context for the service provided by the connection library.
void ConnectionTasks();

// Close USB connection. All existing connections will be gracefully closed.
void ConnectionShutdownAll();

bool ConnectionTypeSupported(CHANNEL_TYPE con);
bool ConnectionCanOpenChannel(CHANNEL_TYPE con);
CHANNEL_HANDLE ConnectionOpenChannelAccessory(ChannelReceiveCallback cb);
CHANNEL_HANDLE ConnectionOpenChannelCdc(ChannelReceiveCallback cb);
void ConnectionSend(CHANNEL_HANDLE ch, const uint8_t *data, int size);
bool ConnectionCanSend(CHANNEL_HANDLE ch);
void ConnectionCloseChannel(CHANNEL_HANDLE ch);
int ConnectionGetMaxPacket(CHANNEL_HANDLE ch);

#endif  // __CONNECTION_H__
