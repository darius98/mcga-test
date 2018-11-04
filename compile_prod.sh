#!/usr/bin/env bash

cmake -Hkktest_lib -Bbuild/cmake_prod -DBUILD_TYPE:STRING=PROD
cd build/cmake_prod && make
