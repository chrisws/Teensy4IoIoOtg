#!/bin/bash

DIR=`pwd`

git submodule init
git submodule update

(cd modules/CMSIS-DSP && \
    mkdir -p build && \
    cd build && \
    cmake .. -Wno-dev -DARM_CPU=cortex-m7 -DUSE_FPU=ON -DFLOAT_ABI=hard -DCMAKE_C_FLAGS="-I${DIR}/modules/CMSIS_5/CMSIS/Core/Include" && \
    cmake --build .)

(cd modules/teensy_loader_cli && make)

(mkdir -p build && cd build && cmake .. && make)
