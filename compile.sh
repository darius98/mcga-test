#!/usr/bin/env bash

cmake -H. -Bbuild/cmake_dev -DCMAKE_BUILD_TYPE=Debug -DBUILD_TYPE:STRING=DEV
cd build/cmake_dev && make -j4 && cd ../..
