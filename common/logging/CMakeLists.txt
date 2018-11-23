cmake_minimum_required(VERSION 3.7)
project(Logging VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(logging src/test_logger.cpp)
target_include_directories(logging PUBLIC include)
target_include_directories(logging PRIVATE include/logging_public)
