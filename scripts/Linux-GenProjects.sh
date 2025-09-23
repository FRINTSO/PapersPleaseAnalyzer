#!/bin/bash

pushd ..
./vendor/premake/bin/premake5 --cc=clang --file=premake5.lua gmake
popd
