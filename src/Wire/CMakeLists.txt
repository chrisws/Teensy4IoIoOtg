#
# Teensy4 IOIO-OTG Project
#
# This project is a port of the IOIO-OTG firmware to the Teensy 4.x platform.
#
# Author: Chris Warren-Smith
# Based on original work by: Ytai Ben-Tsvi (IOIO-OTG)
# License: Apache License, Version 2.0
#

set(SRC "../../modules/Wire")

include_directories(${SRC})

set(SOURCES
 ${SRC}/Wire.cpp
 ${SRC}/WireIMXRT.cpp
 ${SRC}/WireKinetis.cpp
)

add_library(Wire STATIC ${SOURCES})
