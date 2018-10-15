#!/usr/bin/env bash

git submodule update --init --recursive

# Copy files to include folder
sudo cp -r kktest_lib/ /usr/local/include/kktest_src
sudo cp -r third_party/ /usr/local/include/kktest_src/third_party
sudo cp kktest_lib/src/kktest /usr/local/include/kktest
sudo cp kktest_lib/src/kktest_matchers /usr/local/include/kktest_matchers
sudo cp kktest_lib/kktest_library.cmake /usr/local/include/kktest.cmake

# Install and init box
cd third_party/sandman
sudo apt install -y acl
make build
sudo mv box /usr/local/bin/box
sudo mkdir -p /eval
sudo touch /eval/isolate.log
sudo box --init
