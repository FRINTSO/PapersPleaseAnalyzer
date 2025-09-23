#!/bin/bash

set -e
set -x

VENDOR_DIR=$(pwd)
OCV_DIR="$VENDOR_DIR/opencv"

# 1. Clone opencv repo
if [[ ! -d $OCV_DIR  ]]; then
    git clone --depth 1 https://github.com/opencv/opencv.git
else
    pushd opencv
    git pull --rebase
    popd
fi

# 2. Create a staging area/build area
# opencv-build
BUILD_DIR="$VENDOR_DIR/opencv-build"
mkdir -p "$BUILD_DIR"
# opencv-install
INSTALL_DIR="$VENDOR_DIR/opencv-install"
mkdir -p "$INSTALL_DIR"

# 3. Build opencv with correct flags
(
    cd "$BUILD_DIR"

    CMAKE_GENERATOR_OPTIONS=(-G "Visual Studio 17 2022" -A x64)
    CMAKE_OPTIONS=(
        -DBUILD_PERF_TESTS:BOOL=OFF
        -DBUILD_TESTS:BOOL=OFF
        -DBUILD_DOCS:BOOL=OFF
        -DWITH_CUDA:BOOL=OFF
        -DBUILD_EXAMPLES:BOOL=OFF
        -DINSTALL_CREATE_DISTRIB=ON
        -DBUILD_SHARED_LIBS=OFF

        -DBUILD_opencv_world=ON
        -DBUILD_opencv_core=ON
        -DBUILD_opencv_imgproc=ON
        -DBUILD_opencv_imgcodecs=ON
        -DBUILD_opencv_highgui=ON
        -DBUILD_opencv_videoio=ON
        -DBUILD_opencv_python3=OFF
    )

    cmake "${CMAKE_GENERATOR_OPTIONS[@]}" "${CMAKE_OPTIONS[@]}" -DCMAKE_INSTALL_PREFIX="$INSTALL_DIR" ../opencv

    # 4. Install opencv debug/release into opencv-build folder
    cmake --build .  --config Debug
    cmake --build .  --config Release
    cmake --build .  --target install --config Release
    cmake --build .  --target install --config Debug
)

# 5. Cleanup everything except for .dll/lib files and include/ directory (only what's needed shall be left)
# TODO: Optional
