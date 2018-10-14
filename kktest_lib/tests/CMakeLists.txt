cmake_minimum_required(VERSION 3.7)

set(BIN_PATH   "${CMAKE_CURRENT_LIST_DIR}/../../cmake_build/bin")
set(LIB_PATH   "${CMAKE_CURRENT_LIST_DIR}/../../cmake_build/lib")
set(TESTS_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake_build/tests")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

include("/usr/local/include/kktest.cmake")

set(BASE_MATCHER_CPPS
        ../src/matcher/matcher.cpp
        ../src/matcher/description.cpp
        ../src/matcher/expect_matches.cpp)
add_library(base_matcher_lib ${BASE_MATCHER_CPPS})
target_include_directories(base_matcher_lib PUBLIC ../src)

testcase(truth_matcher_test matcher/truth_matcher_test.cpp ../src/matcher/truth_matcher.cpp base_matcher_lib)
testcase(comparison_matcher_test matcher/comparison_matcher_test.cpp base_matcher_lib)
