# Teensy4IoIoOtg

This project is a port of the IOIO-OTG firmware to the Teensy 4.x platform.

## Setup

1. Install tools

```
sudo apt install gcc-arm-none-eabi libusb-dev tio
```

2. Run `setup.sh`

3. Setup UDEV rules


```
curl -sLO https://www.pjrc.com/teensy/00-teensy.rules
sudo cp 00-teensy.rules /etc/udev/rules.d/
```

4. Invoke `run.sh` to load the firmware onto your teensy device.

