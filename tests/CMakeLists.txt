cmake_minimum_required(VERSION 3.7)

set(BIN_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/../build/bin")
set(LIB_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/../build/lib")
set(TESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/tests")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

include("../kktest_library.cmake")

matcher_testcase(truth_matcher matcher/truth_matcher_test.cpp)
