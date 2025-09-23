#!/usr/bin/env bash
set -e
set -x

# --- 0. Config ---
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
VENDOR_DIR="$SCRIPT_DIR/../Paplease-Core/vendor"             # parent folder for all vendored stuff
OPENCV_REPO_DIR="$VENDOR_DIR/opencv"        # OpenCV repo folder
BUILD_DIR="$VENDOR_DIR/opencv-build"
STAGING_DIR="$VENDOR_DIR/opencv-build/install"

# --- 1. Clone OpenCV repo if not already ---
if [ ! -d "$OPENCV_REPO_DIR" ]; then
    echo "Cloning OpenCV $OPENCV_VERSION..."
    mkdir -p "$VENDOR_DIR"
    git clone --depth 1 https://github.com/opencv/opencv.git "$OPENCV_REPO_DIR"
fi

# --- 2. Create staging/build area ---
rm -rf "$BUILD_DIR" "$STAGING_DIR"
mkdir -p "$BUILD_DIR" "$STAGING_DIR"

# --- 3. Build OpenCV ---
cd "$BUILD_DIR"

#-DBUILD_LIST=core,imgproc,highgui,world \
cmake "$VENDOR_DIR/opencv" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX="$STAGING_DIR" \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_PERF_TESTS=OFF \
    -DBUILD_TESTS=OFF \
    -DBUILD_DOCS=OFF \
    -DWITH_CUDA=OFF \
    -DINSTALL_CREATE_DISTRIB=ON \
    -DBUILD_opencv_world=ON \
    -DBUILD_opencv_core=ON \
    -DBUILD_opencv_imgproc=ON \
    -DBUILD_opencv_imgcodecs=ON \
    -DBUILD_opencv_highgui=ON

cmake --build . --config Release --parallel
cmake --build . --target install --config Release --parallel

# Optionally build Debug as well
cmake "$VENDOR_DIR/opencv" \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX="$STAGING_DIR"

cmake --build . --config Debug --parallel
cmake --build . --target install --config Debug --parallel

# --- 4. Cleanup unneeded files ---
echo "Cleaning up unnecessary files..."
cd "$STAGING_DIR"

# Keep only include, lib, bin directories
find . -type f ! \( -name "*.dll" -o -name "*.so*" -o -name "*.dylib" -o -name "*.lib" -o -path "./include/*" \) -delete
find . -type d -empty -delete

echo "OpenCV build completed at $STAGING_DIR"
