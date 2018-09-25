cmake_minimum_required(VERSION 3.8)
project(RuntimeTesting)
set(CMAKE_CXX_STANDARD 14)

file(GLOB MatchersSrc src/matcher/*.hpp src/matcher/*.cpp)
file(GLOB TestingSrc src/testing/*.hpp src/testing/*.cpp)
file(GLOB UtilsSrc src/utils/*.hpp src/utils/*.cpp)
file(GLOB AllSrc ${MatchersSrc} ${TestingSrc} ${UtilsSrc})

file(GLOB AllTests tests/all_tests.cpp tests/**/*.cpp tests/**/*.hpp)

add_executable(TestAll ${AllTests} ${AllSrc})

include_directories(.)

add_executable(MatcherConcept concept/expect.cpp runtime_testing ${AllSrc})
add_executable(TestingConcept concept/runtime_testing.cpp runtime_testing ${AllSrc})
