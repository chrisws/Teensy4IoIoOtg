#!/bin/bash

(cd build && make) && \
    ./modules/teensy_loader_cli/teensy_loader_cli --mcu=IMXRT1062 -w -v build/firmware.hex

#sleep 1 && \
#    miniterm /dev/ttyACM0 115200
