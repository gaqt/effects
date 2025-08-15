#!/bin/bash

mkdir -p dist/lib
cd lib/raylib/src
make PLATFORM=PLATFORM_DESKTOP
cd ../../..
mv lib/raylib/src/libraylib.a dist/lib/libraylib.a
