cmake_minimum_required(VERSION 3.7)
project(KKTestRunner VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 14)

include("../kktest_lib/kktest.cmake")

add_executable(kktest_runner entry_point.cpp)
target_link_libraries(kktest_runner autojson easyflags)
