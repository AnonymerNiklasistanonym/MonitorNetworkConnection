#!/usr/bin/env bash

mkdir -p build && cd build || exit 1
cmake ..
make -j"$(nproc)"
./dataAnalysis
