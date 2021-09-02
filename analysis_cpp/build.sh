#!/usr/bin/env bash

set -e

mkdir -p build && cd build || exit 1
cmake ..
make -j"$(nproc)"

./dataAnalysis
