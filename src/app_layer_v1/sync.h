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

// Synchronization utilities.

#ifndef __SYNC_H__
#define __SYNC_H__

#include <cstdint>

// Disable interrupts at or below a certain level.
// This is intended to be followed by a statement or a block, within which the
// masking will apply. Once the block exits, even when jumping out in the middle
// of it, the masking will be returned to its previous value.
//
// Example:
// PRIORITY(5) {  // disable interrupt with priority <= 5
//   ... do something critical ...
// } // return to previous state

void SyncInit();

struct DisableInterrupts {
  explicit DisableInterrupts(unsigned _priority);
  ~DisableInterrupts();
  bool latch();

  private:
  unsigned _priority;
  bool _once;
};

#define PRIORITY(pri) for (DisableInterrupts disableInterrupts(pri); disableInterrupts.latch();)

#endif  // __SYNC_H__
