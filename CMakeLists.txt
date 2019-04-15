cmake_minimum_required(VERSION 3.12.4)
project(MCGA_test)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

option(MCGA_test_tests "Build MCGA Test tests" OFF)

include_directories(.)

add_subdirectory(third_party/termcolor EXCLUDE_FROM_ALL)

add_subdirectory(common/interproc)
add_subdirectory(common/utils)
add_subdirectory(kktest)

if (MCGA_test_tests)
    enable_testing()
    add_subdirectory(tests)
endif ()
