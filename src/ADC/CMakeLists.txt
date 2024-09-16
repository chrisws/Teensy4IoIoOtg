#
# Teensy4 IOIO-OTG Project
#
# This project is a port of the IOIO-OTG firmware to the Teensy 4.x platform.
#
# Author: Chris Warren-Smith
# Based on original work by: Ytai Ben-Tsvi (IOIO-OTG)
# License: Apache License, Version 2.0
#

set(SRC "../../modules/ADC")

include_directories(${SRC})

set(SOURCES
 ${SRC}/ADC.cpp
 ${SRC}/ADC_Module.cpp
 ${SRC}/AnalogBufferDMA.cpp
)

add_library(ADC STATIC ${SOURCES})

target_include_directories(ADC PRIVATE ${CMAKE_SOURCE_DIR}/modules/cores/teensy4)
