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

#ifndef __FIELD_ACCESSORS_H__
#define __FIELD_ACCESSORS_H__

void AssignUxRXIF(unsigned index, unsigned value);
void AssignUxRXIE(unsigned index, unsigned value);
void AssignUxRXIP(unsigned index, unsigned value);
void AssignUxTXIF(unsigned index, unsigned value);
void AssignUxTXIE(unsigned index, unsigned value);
void AssignUxTXIP(unsigned index, unsigned value);
void AssignICxIF(unsigned index, unsigned value);
void AssignICxIE(unsigned index, unsigned value);
void AssignICxIP(unsigned index, unsigned value);
void AssignSPIxIF(unsigned index, unsigned value);
void AssignSPIxIE(unsigned index, unsigned value);
void AssignSPIxIP(unsigned index, unsigned value);
void AssignMI2CxIF(unsigned index, unsigned value);
void AssignMI2CxIE(unsigned index, unsigned value);
void AssignMI2CxIP(unsigned index, unsigned value);

#endif  // __FIELD_ACCESSORS_H__
