#!/bin/bash

# Setup git submodules
git submodule update --init --recursive


# Build opencv
(
    cd ../Paplease-Core/vendor
    mkdir -p build && cd build

    cmake ../opencv

    cmake --build .
)
