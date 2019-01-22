#!/usr/bin/env bash

mkdir -p generated

cp kktest_manager/templates/build_kktest.cmake.template generated/build_kktest.cmake
cp kktest_manager/templates/kktest_main.cpp.template generated/kktest_main.cpp

cmake -H. -Bcmake_build -DCMAKE_BUILD_TYPE=Release
cd cmake_build
make
sudo make install
cd ..
