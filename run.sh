#!/bin/bash

function run() {
    sleep 1 && tio /dev/serial/by-id/usb-Teensyduino_USB_Serial_15400590-if00
}

(cd build && make) && \
    ./modules/teensy_loader_cli/teensy_loader_cli --mcu=IMXRT1062 -w -v build/firmware.hex && run
