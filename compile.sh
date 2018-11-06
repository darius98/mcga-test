#!/usr/bin/env bash

cmake -H. -Bbuild/cmake -DCMAKE_BUILD_TYPE=$1 -DBUILD_TYPE:STRING=$2
cd build/cmake && make -j4 && cd ../..
