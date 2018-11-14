#!/usr/bin/env bash

git submodule update --init --recursive

./compile.sh Debug DEV cmake_dev
