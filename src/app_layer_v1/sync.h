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

#define PRIORITY(pri)

/* for (unsigned _sr __attribute__((cleanup(RestoreSR))) = SR, _i = 1; \ */
/*      _i && (SR = (pri << 5));                                       \ */
/*      _i = 0) */

/* // Do not call directly. */
/* static inline void RestoreSR(unsigned const *sr) { */
/*   SR = *sr; */
/* } */

// Step 1: Define the Macro
// Create a macro for managing interrupt priorities. You might need to adjust the priority values to match the range expected by the ARM Cortex-M NVIC.
// 
// #include "Arduino.h"  // Include this for Teensy or other ARM CMSIS headers
// 
// // Define a macro for managing interrupt priority on Teensy
// #define PRIORITY(n)                                     \
//   uint32_t old_priority = NVIC_GetPriority(SysTick_IRQn); \
//   NVIC_SetPriority(SysTick_IRQn, (n)); \
//   __disable_irq(); \
//   { \
// 
// #define END_PRIORITY                          \
//   } \
//   NVIC_SetPriority(SysTick_IRQn, old_priority); \
//   __enable_irq();
// Step 2: Use the Macro in Your Code
// You would then use these macros in your code like so:
// 
// cpp
// Copy code
// PRIORITY(5) {  // disable interrupt with priority <= 5
//   // ... do something critical ...
// } END_PRIORITY
// Explanation
// NVIC_GetPriority: This function retrieves the current priority of the specified interrupt. You can use SysTick_IRQn or the specific IRQ you are interested in.
// NVIC_SetPriority: This sets the priority of the specified interrupt.
// __disable_irq: Disables global interrupts to ensure that the critical section runs atomically.
// __enable_irq: Re-enables global interrupts.
// Important Considerations
// Adjust Priority Levels: Ensure that the priority levels you are using are within the range defined by the ARM Cortex-M specifications. The priority level range is typically from 0 to 255, but it’s often configured in steps of 4 or 8 (e.g., 0-15 with 4-bit priority).
// 
// Choose the Correct IRQ Number: SysTick_IRQn is used here as a placeholder. For specific interrupts, replace SysTick_IRQn with the appropriate interrupt number.
// 
// Check CMSIS Documentation: Consult the CMSIS (Cortex Microcontroller Software Interface Standard) documentation for more details on handling interrupts for your specific Teensy model.
// 
// By defining the appropriate macros and using them correctly, you can manage interrupt priorities and critical sections effectively on the Teensy microcontroller.
// 

#endif  // __SYNC_H__
