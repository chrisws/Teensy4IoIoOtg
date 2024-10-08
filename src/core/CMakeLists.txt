#
# Teensy4 IOIO-OTG Project
#
# This project is a port of the IOIO-OTG firmware to the Teensy 4.x platform.
#
# Author: Chris Warren-Smith
# Based on original work by: Ytai Ben-Tsvi (IOIO-OTG)
# License: Apache License, Version 2.0
#

set(SRC ${CMAKE_SOURCE_DIR}/modules/cores/teensy4)

include_directories(${SRC})

set(SOURCES
 ${SRC}/analog.c
 ${SRC}/bootdata.c
 ${SRC}/clockspeed.c
 ${SRC}/CrashReport.cpp
 ${SRC}/debugprintf.c
 ${SRC}/delay.c
 ${SRC}/digital.c
 ${SRC}/DMAChannel.cpp
 ${SRC}/eeprom.c
 ${SRC}/EventResponder.cpp
 ${SRC}/extmem.c
 ${SRC}/fuse.c
 ${SRC}/HardwareSerial1.cpp
 ${SRC}/HardwareSerial2.cpp
 ${SRC}/HardwareSerial3.cpp
 ${SRC}/HardwareSerial4.cpp
 ${SRC}/HardwareSerial5.cpp
 ${SRC}/HardwareSerial6.cpp
 ${SRC}/HardwareSerial7.cpp
 ${SRC}/HardwareSerial8.cpp
 ${SRC}/HardwareSerial.cpp
 ${SRC}/interrupt.c
 ${SRC}/IntervalTimer.cpp
 ${SRC}/IPAddress.cpp
 ${SRC}/keylayouts.c
 ${SRC}/libc.c
 ${SRC}/new.cpp
 ${SRC}/nonstd.c
 ${SRC}/Print.cpp
 ${SRC}/pwm.c
 ${SRC}/rtc.c
 ${SRC}/serialEvent1.cpp
 ${SRC}/serialEvent2.cpp
 ${SRC}/serialEvent3.cpp
 ${SRC}/serialEvent4.cpp
 ${SRC}/serialEvent5.cpp
 ${SRC}/serialEvent6.cpp
 ${SRC}/serialEvent7.cpp
 ${SRC}/serialEvent8.cpp
 ${SRC}/serialEvent.cpp
 ${SRC}/serialEventUSB1.cpp
 ${SRC}/serialEventUSB2.cpp
 ${SRC}/sm_alloc_valid.c
 ${SRC}/sm_calloc.c
 ${SRC}/sm_free.c
 ${SRC}/sm_hash.c
 ${SRC}/sm_malloc.c
 ${SRC}/sm_malloc_stats.c
 ${SRC}/sm_pool.c
 ${SRC}/sm_realloc.c
 ${SRC}/sm_realloc_i.c
 ${SRC}/sm_realloc_move.c
 ${SRC}/sm_szalloc.c
 ${SRC}/sm_util.c
 ${SRC}/sm_zalloc.c
 ${SRC}/startup.c
 ${SRC}/Stream.cpp
 ${SRC}/tempmon.c
 ${SRC}/Time.cpp
 ${SRC}/usb.c
 ${SRC}/usb_desc.c
 ${SRC}/usb_flightsim.cpp
 ${SRC}/usb_inst.cpp
 ${SRC}/usb_joystick.c
 ${SRC}/usb_keyboard.c
 ${SRC}/usb_midi.c
 ${SRC}/usb_mouse.c
 ${SRC}/usb_mtp.c
 ${SRC}/usb_rawhid.c
 ${SRC}/usb_seremu.c
 ${SRC}/usb_serial.c
 ${SRC}/usb_serial2.c
 ${SRC}/usb_serial3.c
 ${SRC}/usb_serial.c
 ${SRC}/usb_touch.c
 ${SRC}/WMath.cpp
 ${SRC}/WString.cpp
 ${SRC}/yield.cpp
 ${SRC}/main.cpp
)

add_library(core STATIC ${SOURCES})
