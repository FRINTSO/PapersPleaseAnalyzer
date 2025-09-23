#!/bin/bash -e

# Script
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

# Root of your repo
myRepo="$SCRIPT_DIR/../Paplease-Core/vendor"
RepoSource=opencv
BuildDir=build_opencv
InstallDir="$myRepo/install/$RepoSource"

# -------------------------------
# Step 1: Clone/update OpenCV repo
# -------------------------------
if [ ! -d "$myRepo/opencv" ]; then
    echo "[INFO] Cloning OpenCV..."
    git clone https://github.com/opencv/opencv.git
else
    echo "[INFO] Updating OpenCV..."
    (cd opencv && git pull --rebase)
fi

# -------------------------------
# Step 2: Choose generator
# -------------------------------
# Linux/Unix defaults to Ninja
# Windows defaults to Visual Studio 2022 x64
if [[ "$OS" == "Windows_NT" ]]; then
    CMAKE_GENERATOR_OPTIONS=(-G "Visual Studio 17 2022" -A x64)
else
    CMAKE_GENERATOR_OPTIONS=(-G "Ninja")
fi

# -------------------------------
# Step 3: Build options (minimal modules)
# -------------------------------
CMAKE_OPTIONS=(
    -DBUILD_PERF_TESTS=OFF
    -DBUILD_TESTS=OFF
    -DBUILD_DOCS=OFF
    -DBUILD_EXAMPLES=OFF
    -DWITH_CUDA=OFF
    -DINSTALL_CREATE_DISTRIB=ON
    -DBUILD_opencv_world=ON
    -DBUILD_opencv_core=ON
    -DBUILD_opencv_imgproc=ON
    -DBUILD_opencv_imgcodecs=ON
    -DBUILD_opencv_highgui=ON
    -DBUILD_opencv_videoio=ON
    -DBUILD_opencv_python3=OFF
)

# -------------------------------
# Step 4: Configure + Build
# -------------------------------
mkdir -p "$BuildDir"
pushd "$BuildDir"

set -x
cmake "${CMAKE_GENERATOR_OPTIONS[@]}" "${CMAKE_OPTIONS[@]}" \
    -DCMAKE_INSTALL_PREFIX="$InstallDir" \
    "$myRepo/$RepoSource"

echo "************************* Debug build"
cmake --build . --config Debug --parallel

echo "************************* Release build"
cmake --build . --config Release --parallel

echo "************************* Install Release"
cmake --build . --target install --config Release --parallel

echo "************************* Install Debug"
cmake --build . --target install --config Debug --parallel

popd

echo "[DONE] OpenCV built and installed to $InstallDir"