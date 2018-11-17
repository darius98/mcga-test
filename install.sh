#!/usr/bin/env bash

git submodule update --init --recursive

./compile.sh Release PROD cmake_prod

# Remove any previous version
./uninstall.sh

sudo cp build/lib/libkktest.so   /usr/local/lib/libkktest.so
sudo cp build/lib/libkktest.so   /usr/lib/libkktest.so
sudo cp build/lib/libkktest.so.1 /usr/local/lib/libkktest.so.1
sudo cp build/lib/libkktest.so.1 /usr/lib/libkktest.so.1

# Copy public include files to include folder
sudo cp -r lib/include/* /usr/local/include

# Copy the cmake library
sudo cp lib/kktest.cmake /usr/local/include/kktest.cmake

# Copy the runner binary to a binary path
sudo cp build/bin/runner /usr/local/bin/kktest
