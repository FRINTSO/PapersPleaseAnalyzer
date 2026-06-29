#!/bin/sh

./vendor/premake/bin/premake5 --cc=clang --file=premake5.lua gmake

bear -- make -B -j8
CC=clang CXX=clang++ cmake -S . -B build --fresh && cmake --build build -j$(nproc)
