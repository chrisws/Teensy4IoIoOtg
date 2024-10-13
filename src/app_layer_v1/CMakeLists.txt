#
# Teensy4 IOIO-OTG Project
#
# This project is a port of the IOIO-OTG firmware to the Teensy 4.x platform.
#
# Author: Chris Warren-Smith
# Based on original work by: Ytai Ben-Tsvi (IOIO-OTG)
# License: Apache License, Version 2.0
#

set(SOURCES
  adc.cpp
  digital.cpp
  features.cpp
  i2c.cpp
  icsp.cpp
  incap.cpp
  pins.cpp
  protocol.cpp
  pwm.cpp
  sequencer.cpp
  sequencer_protocol.cpp
  spi.cpp
  sync.cpp
  timers.cpp
  uart.cpp
  logging.cpp
  byte_queue.cpp
  connection.cpp
  usb_android.cpp
)

add_library(app_layer_v1 STATIC ${SOURCES})

target_compile_options(app_layer_v1 PRIVATE -Wall)
target_include_directories(app_layer_v1 PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(app_layer_v1 PRIVATE ${CMAKE_SOURCE_DIR}/modules/cores/teensy4)
target_include_directories(app_layer_v1 PRIVATE ${CMAKE_SOURCE_DIR}/modules/ADC)
target_include_directories(app_layer_v1 PRIVATE ${CMAKE_SOURCE_DIR}/modules/SPI)
target_include_directories(app_layer_v1 PRIVATE ${CMAKE_SOURCE_DIR}/modules/Wire)
target_include_directories(app_layer_v1 PRIVATE ${CMAKE_SOURCE_DIR}/modules/USBHost_t36)
target_include_directories(app_layer_v1 PRIVATE ${CMAKE_SOURCE_DIR}/modules/SdFat/src)

