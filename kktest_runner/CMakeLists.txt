cmake_minimum_required(VERSION 3.7)
project(KKTestRunner VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 14)

include("../kktest_lib/kktest.cmake")

file(GLOB UTILS_SRC utils/*.hpp utils/*.cpp)
file(GLOB OS_SRC os/*.hpp os/*.cpp)
add_executable(kktest_runner entry_point.cpp ${UTILS_SRC} ${OS_SRC})
target_include_directories(kktest_runner PRIVATE .)
target_link_libraries(kktest_runner autojson easyflags)
