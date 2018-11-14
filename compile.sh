#!/usr/bin/env bash

cmake -H. -Bbuild/$3 -DCMAKE_BUILD_TYPE=$1 -DBUILD_TYPE:STRING=$2
cd build/$3 && make -j4 && cd ../..
