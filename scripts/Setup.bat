@echo off
setlocal enabledelayedexpansion

REM === Setup git submodules ===
git submodule update --init --recursive

REM === Build OpenCV ===
cd ..\Paplease-Core\vendor

if not exist build (
    mkdir build
)

cd build

cmake ..\opencv

cmake --build . --parallel

endlocal
