cmake_minimum_required(VERSION 3.12)
project(RuntimeTesting)
set(CMAKE_CXX_STANDARD 14)

file(GLOB MatchersSrc src/matcher/*.hpp src/matcher/*.cpp)
file(GLOB TestingSrc src/testing/*.hpp src/testing/*.cpp)
file(GLOB UtilsSrc src/utils/*.hpp src/utils/*.cpp)
file(GLOB AllSrc ${MatchersSrc} ${TestingSrc} ${UtilsSrc})

add_executable(TestTruthMatchers
        tests/matchers/truth_matcher_test.cpp
        tests/matchers/matcher_test.hpp
        ${MatchersSrc}
)

add_executable(TestComparisonMatchers
        tests/matchers/comparison_matcher_test.cpp
        tests/matchers/matcher_test.hpp
        ${MatchersSrc}
)
