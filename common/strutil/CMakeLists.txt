cmake_minimum_required(VERSION 3.7)
project(StrUtil VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(strutil
    src/casing.cpp
    src/string_key_comparable.cpp
    src/substr_utils.cpp
    src/unescape.cpp
)
target_include_directories(strutil PUBLIC include)
target_include_directories(strutil PRIVATE include/strutil_public)
