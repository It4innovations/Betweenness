#!/bin/bash

ml intel/2018.03 CMake

mkdir Code/build
cd Code/build || exit 1
cmake ..
make -j 24 VERBOSE=1
