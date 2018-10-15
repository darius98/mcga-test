#!/usr/bin/env bash

cmake -H./ -Bcmake_build/cmake
cd cmake_build/cmake && make
