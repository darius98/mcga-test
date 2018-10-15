cmake_minimum_required(VERSION 3.7)

set(TESTS_PATH "${CMAKE_CURRENT_LIST_DIR}/../../cmake_build/tests")

if(DEV)
    include("../kktest_library.cmake")
else()
    include("/usr/local/include/kktest.cmake")
endif()

set(BASE_MATCHER_CPPS
        ../src/matcher/matcher.cpp
        ../src/matcher/description.cpp
        ../src/matcher/expect_matches.cpp)
add_library(base_matcher_lib ${BASE_MATCHER_CPPS})
target_include_directories(base_matcher_lib PUBLIC ../src)

testcase(truth_matcher_test matcher/truth_matcher_test.cpp ../src/matcher/truth_matcher.cpp base_matcher_lib)
testcase(comparison_matcher_test matcher/comparison_matcher_test.cpp base_matcher_lib)
