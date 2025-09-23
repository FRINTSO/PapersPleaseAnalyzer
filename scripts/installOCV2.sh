#!/bin/bash
set -e
set -x

# Script
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

vendorDir="$SCRIPT_DIR/../Paplease-Core/vendor"

(
    cd $vendorDir

    git clone https://github.com/opencv/opencv.git

    mkdir -p build && cd build

    # CMAKE
    pushd build_opencv
    CMAKE_GENERATOR_OPTIONS=(-G"Visual Studio 17 2022" -A x64)

    CMAKE_OPTIONS=(
        -DBUILD_PERF_TESTS:BOOL=OFF
        -DBUILD_TESTS:BOOL=OFF
        -DBUILD_DOCS:BOOL=OFF
        -DBUILD_EXAMPLES:BOOL=OFF
        -DWITH_CUDA:BOOL=OFF
        -DINSTALL_CREATE_DISTRIB=OFF # ON
    )

    set -x
    cmake "${CMAKE_GENERATOR_OPTIONS[@]}" "${CMAKE_OPTIONS[@]}" ../opencv-4.x # -DCMAKE_INSTALL_PREFIX="$myRepo/install/$RepoSource" "$myRepo/$RepoSource"

    echo "************************* Debug build"
    cmake --build . --config Debug --parallel

    echo "************************* Release build"
    cmake --build . --config Release --parallel

    echo "************************* Install Release"
    cmake --build . --target install --config Release --parallel

    echo "************************* Install Debug"
    cmake --build . --target install --config Debug --parallel
    popd
)