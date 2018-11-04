cmake_minimum_required(VERSION 3.7)

set(TESTS_PATH "${CMAKE_CURRENT_LIST_DIR}/../../build/tests")

if(DEV)
    include("../kktest.cmake")
else()
    include("/usr/local/include/kktest.cmake")
endif()

add_kktest(truth_matcher_test      matcher/truth_matcher_test.cpp)
add_kktest(comparison_matcher_test matcher/comparison_matcher_test.cpp)
add_kktest(numeric_matchers_test   matcher/numeric_matchers_test.cpp)
