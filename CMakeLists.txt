cmake_minimum_required(VERSION 3.8)
project(RuntimeTesting)
set(CMAKE_CXX_STANDARD 14)

include_directories(.)
file(GLOB RuntimeTestingLibImpl src/**/*.cpp src/**/*.hpp runtime_testing)
add_library("runtime_testing" ${RuntimeTestingLibImpl})

add_executable(StdVectorTests concept/std_vector_test.cpp)
install(TARGETS StdVectorTests DESTINATION StdVectorTests)
target_link_libraries(StdVectorTests "runtime_testing")

add_executable(WikiExampleBase wiki_examples/base.cpp)
install(TARGETS WikiExampleBase DESTINATION WikiExampleBase)
target_link_libraries(WikiExampleBase "runtime_testing")
