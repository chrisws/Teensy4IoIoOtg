#
# CMakeLists.txt - Teensy4 IOIO-OTG Project
#
# This project is a port of the IOIO-OTG firmware to the Teensy 4.x platform.
#
# Author: Chris Warren-Smith
# Based on original work by: Ytai Ben-Tsvi (IOIO-OTG)
# License: Apache License, Version 2.0
#

cmake_minimum_required(VERSION 3.10)
project(Teensy4IoIoOtg)
set(CMAKE_SYSTEM_NAME Generic)
set(TARGET "firmware")

# settings for teensy 4.0
set(MCU "IMXRT1062")
set(MCU_LD ${CMAKE_CURRENT_SOURCE_DIR}/modules/cores/teensy4/imxrt1062.ld)
set(CPU_OPTIONS "-mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16 -mthumb")

# Preprocessor flags for both C and C++
add_compile_options(-DF_CPU=600000000 -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DUSING_MAKEFILE -D__${MCU}__ -DARDUINO=10813 -DTEENSYDUINO=159 -DARDUINO_TEENSY40)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPU_OPTIONS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CPU_OPTIONS} -std=gnu++17 -felide-constructors -fno-exceptions -fpermissive -fno-rtti -Wno-error=narrowing")
set(CMAKE_EXE_LINKER_FLAGS "-Os -Wl,--gc-sections,--relax ${SPECS} ${CPU_OPTIONS} -T ${MCU_LD}")

# Set the toolchain for cross-compilation
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_SIZE arm-none-eabi-size)

# Include the module directories
add_subdirectory(src/core)
add_subdirectory(src/ADC)
add_subdirectory(src/SPI)
add_subdirectory(src/Wire)
add_subdirectory(src/USBHost_t36)
add_subdirectory(src/app_layer_v1)

# Add executable with the list of source files
add_executable(${TARGET}.elf src/main.cpp)

# Link against the CMSIS DSP library
target_link_libraries(${TARGET}.elf app_layer_v1 core ADC SPI Wire USBHost_t36 ${CMAKE_CURRENT_SOURCE_DIR}/tools/CMSIS-DSP/build/Source/libCMSISDSP.a -lm -lstdc++)

# Post-build step to create HEX file using objcopy
add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex -R .eeprom ${TARGET}.elf ${TARGET}.hex
    COMMENT "Generating ${TARGET}.hex from ${TARGET}.elf"
)

# Optional: Show size of the ELF file after building (equivalent to $(SIZE) $<)
add_custom_command(TARGET ${TARGET}.elf POST_BUILD
    COMMAND ${CMAKE_SIZE} ${TARGET}.elf
    COMMENT "Showing size of ${TARGET}.elf"
)
