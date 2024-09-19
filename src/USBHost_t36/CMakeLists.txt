#
# Teensy4 IOIO-OTG Project
#
# This project is a port of the IOIO-OTG firmware to the Teensy 4.x platform.
#
# Author: Chris Warren-Smith
# Based on original work by: Ytai Ben-Tsvi (IOIO-OTG)
# License: Apache License, Version 2.0
#

set(SRC "../../modules/USBHost_t36")

include_directories(${SRC})

set(SOURCES
 ${SRC}/adk.cpp
 ${SRC}/antplus.cpp
 ${SRC}/digitizer.cpp
 ${SRC}/ehci.cpp
 ${SRC}/enumeration.cpp
 ${SRC}/hid.cpp
 ${SRC}/hub.cpp
 ${SRC}/memory.cpp
 ${SRC}/mouse.cpp
 ${SRC}/rawhid.cpp
 ${SRC}/SerEMU.cpp
 ${SRC}/serial.cpp
)

add_library(USBHost_t36 STATIC ${SOURCES})

target_include_directories(USBHost_t36 PRIVATE ${CMAKE_SOURCE_DIR}/modules/cores/teensy4)
