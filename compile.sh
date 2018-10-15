#!/usr/bin/env bash

cmake -H./ -Bbuild/cmake
cd build/cmake && make
