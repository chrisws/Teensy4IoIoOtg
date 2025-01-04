#
# Teensy4 IOIO-OTG Project
#
# This project is a port of the IOIO-OTG firmware to the Teensy 4.x platform.
#
# Author: Chris Warren-Smith
# Based on original work by: Ytai Ben-Tsvi (IOIO-OTG)
# License: Apache License, Version 2.0
#

set(SRC "../../modules/Adafruit_SSD1306")
set(SRC_GFX "../../modules/Adafruit-GFX-Library")

include_directories(${SRC})

set(SOURCES
  lcd_printf.cpp
  ${SRC}/Adafruit_SSD1306.cpp
  ${SRC_GFX}/Adafruit_GFX.cpp
)

add_library(Adafruit_SSD1306 STATIC ${SOURCES})

target_include_directories(Adafruit_SSD1306 PRIVATE ${CMAKE_SOURCE_DIR}/modules/cores/teensy4)
target_include_directories(Adafruit_SSD1306 PRIVATE ${CMAKE_SOURCE_DIR}/modules/Wire)
target_include_directories(Adafruit_SSD1306 PRIVATE ${CMAKE_SOURCE_DIR}/modules/SPI)
target_include_directories(Adafruit_SSD1306 PRIVATE ${CMAKE_SOURCE_DIR}/modules/Adafruit_BusIO)
target_include_directories(Adafruit_SSD1306 PRIVATE ${CMAKE_SOURCE_DIR}/modules/Adafruit-GFX-Library)
