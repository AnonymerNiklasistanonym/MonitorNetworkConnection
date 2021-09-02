#!/usr/bin/env bash

set -e

mkdir -p build && cd build || exit 1
cmake .. -DFORMAT_CODE=ON -DCHECK_CODE=OFF
make -j"$(nproc)"

./dataAnalysis
