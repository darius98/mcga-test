#!/usr/bin/env bash

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

# Install and init box
if ! [ -x "$(command -v box)" ]; then
    sudo apt install -y acl
    git clone https://github.com/establishment/sandman sandman
    cd sandman && make build && cd ..
    sudo mv sandman/box /usr/local/bin/box
    rm -rf sandman
    sudo mkdir -p /eval
    sudo touch /eval/isolate.log
fi
sudo box --init

# Copy the runner binary to a binary path
sudo cp build/bin/runner /usr/local/bin/kktest
