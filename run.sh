#!/bin/bash

function run() {
    # sleep 1 && tio /dev/serial/by-id/usb-Teensyduino_USB_Serial_15400590-if00
    sleep 1 && tio /dev/ttyACM0
}

function show() {
    sleep 1 && lsusb | grep -i teensy

    # socat -d -d pty,link=/dev/ttyV0,raw,echo=0 pty,link=/dev/ttyACM0,raw,echo=0

}

function load() {
    ./modules/teensy_loader_cli/teensy_loader_cli --mcu=IMXRT1062 -w -v build/firmware.hex
}

(cd build && make) && load && show
