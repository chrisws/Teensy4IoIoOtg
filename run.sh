#!/bin/bash

mkdir -p build && (cd build && cmake .. && make)

#	./bin/teensy_loader_cli --mcu=${MCU} -w -v ${TARGET}.elf
#	sleep 1 && miniterm /dev/ttyACM0 115200
