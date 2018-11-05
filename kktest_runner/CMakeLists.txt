cmake_minimum_required(VERSION 3.7)
project(KKTestRunner VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 14)

file(GLOB UTILS_SRC utils/*.hpp utils/*.cpp)
file(GLOB OS_SRC os/*.hpp os/*.cpp)
file(GLOB EXPLORER_SRC explorer/*.hpp explorer/*.cpp)
add_executable(kktest_runner
        entry_point.cpp
        ${UTILS_SRC}
        ${OS_SRC}
        ${EXPLORER_SRC}
    )
target_include_directories(kktest_runner PRIVATE .)
target_link_libraries(kktest_runner autojson easyflags)
