#!/usr/bin/env bash

cmake -H./ -Bbuild/cmake_dev -DBUILD_TYPE:STRING=DEV
cd build/cmake_dev && make
