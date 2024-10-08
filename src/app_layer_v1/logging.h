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

#ifndef __LOGGING_H__
#define __LOGGING_H__

#if defined(ENABLE_LOGGING) || defined(ENABLE_LOGGING_STDIO)
  #include <stdio.h>

  #define STRINGIFY(x) #x
  #define TOSTRING(x) STRINGIFY(x)
  
  void log_print_buf(const void* buf, int size);
  #define log_printf(f, ...) printf("[%s:%d] " f "\r\n", __FILE__, __LINE__, ##__VA_ARGS__)
  void log_init();

  #define SAVE_PIN_FOR_LOG(pin) if (pin == 32) return
  #define SAVE_UART_FOR_LOG(uart) if (uart == 1) return
#else
  #define log_print_buf(b,s)
  #define log_printf(...)
  #define SAVE_PIN_FOR_LOG(pin)
  #define SAVE_UART_FOR_LOG(uart)
  #define log_init()
#endif

#endif  // __LOGGING_H__
