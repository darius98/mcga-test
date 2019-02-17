#!/usr/bin/env bash

# TODO(darius98): Re-write this in Python, and actually parse cmake's output to find the test cases.

rm -rf build/auto_test || { echo 'removing previous build directory failed' ; exit 1; }
mkdir -p build/auto_test || { echo 'making build directory failed' ; exit 1; }
cd build/auto_test || { echo 'moving to build directory failed' ; exit 1; }
cmake ../.. -DCMAKE_BUILD_TYPE=${BUILD_TYPE} || { echo 'CMake generation failed' ; exit 1; }
make -j4 || { echo 'Building tests failed.'; exit 1; }

function fail_build {
    echo building $1 failed.;
    exit 1;
}

function fail_run {
    echo $1 failed.;
    exit 1;
}

function run_tests {
    for var in "$@"
    do
        ../tests/${var}"_test" || fail_run ${var};
    done
}

run_tests common_interproc \
          common_utils \
          kktest_extension_matchers \
          kktest_extension_death
