#!/usr/bin/env bash

# TODO(darius98): Re-write this in Python, and actually parse cmake's output to find the test cases.

mkdir -p build || { echo 'making build directory failed' ; exit 1; }
cd build || { echo 'moving to build directory failed' ; exit 1; }
cmake .. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} || { echo 'CMake generation failed' ; exit 1; }
make -j2 || { echo 'Building tests failed.'; exit 1; }

function fail_build {
    echo building $1 failed.;
    exit 1;
}

function fail_run {
    echo $1 failed.;
    exit 1;
}

function test {
    for var in "$@"
    do
        ./tests/$var"_test" || fail_run $var;
    done
}

test common_interproc \
     common_utils \
     common_string \
     kktest_extension_core_matchers \
     kktest_extension_death
