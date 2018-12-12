#!/usr/bin/env bash

cmake -H. -Bcmake_dev -DCMAKE_BUILD_TYPE=Debug -DBUILD_TYPE:STRING=DEV
cd cmake_dev && make -j4 && cd ..
