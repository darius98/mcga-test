#!/usr/bin/env bash

export CXX=/usr/bin/clang++-3.9
cmake -H. -Bbuild/$3 -DCMAKE_BUILD_TYPE=$1 -DBUILD_TYPE:STRING=$2
cd build/$3 && make -j4 && cd ../..
