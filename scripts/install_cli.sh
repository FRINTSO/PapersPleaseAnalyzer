#!/bin/bash
set -e
set -x
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BUILT_EXE_DIR="$SCRIPT_DIR/../bin/windows-x86_64/Debug/Paplease-CLI"
TARGET_DIR="/mnt/c/tools"

# Rename output
(
	cd "$BUILT_EXE_DIR"
#	cp Paplease-CLI.exe "$TARGET_DIR"
#   mv "$TARGET_DIR/Paplease-CLI.exe" "$TARGET_DIR/paplease.exe"
    mv Paplease-CLI.exe "$TARGET_DIR/paplease.exe"

	cp paplease.exe "$TARGET_DIR"
	cp opencv_world4130.dll "$TARGET_DIR"
	cp opencv_world4130d.dll "$TARGET_DIR"
)


