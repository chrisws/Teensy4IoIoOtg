# Teensy4IoIoOtg

Attempting to port the ioio-otg firmware to the teensy 4.0 board

## Setup

1. Install tools

```
sudo apt install gcc-arm-none-eabi libusb-dev
```

2. Clone and build teensy_loader_cli

```
git clone https://github.com/PaulStoffregen/teensy_loader_cli.git
```

3. Clone and build libCMSISDSP.a (aka libarm_cortexM7lfsp_math.a)

```
cd tools
git clone https://github.com/ARM-software/CMSIS_5.git
git clone https://github.com/ARM-software/CMSIS-DSP.git
cd CMSIS-DSP
mkdir build && cd build
cmake .. -Wno-dev -DARM_CPU=cortex-m7 -DUSE_FPU=ON -DFLOAT_ABI=hard -DCMAKE_C_FLAGS="-I/home/chrisws/src/Teensy4IoIo-otg/tools/CMSIS_5/CMSIS/Core/Include"
cmake --build .
```

4. Build the project

```
mkdir build && cd build
cmake ..
make
```

5. Setup UDEV rules

https://www.pjrc.com/teensy/00-teensy.rules

```
sudo cp 00-teensy.rules /etc/udev/rules.d/
```
