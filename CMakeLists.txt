cmake_minimum_required(VERSION 3.8)
project(RuntimeTesting)
set(CMAKE_CXX_STANDARD 14)

file(GLOB MatchersSrc src/matcher/*.hpp src/matcher/*.cpp)
file(GLOB TestingSrc src/testing/*.hpp src/testing/*.cpp)
file(GLOB UtilsSrc src/utils/*.hpp src/utils/*.cpp)
file(GLOB AllSrc ${MatchersSrc} ${TestingSrc} ${UtilsSrc})

include_directories(.)

add_executable(StdVectorTests concept/std_vector_test.cpp runtime_testing ${AllSrc})
