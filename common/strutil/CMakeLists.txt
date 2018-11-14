cmake_minimum_required(VERSION 3.7)
project(strutillib VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(strutillib
    include/strutil
    include/casing.hpp
    include/string_key_comparable.hpp
    include/substr_utils.hpp
    include/unescape.hpp
    src/casing.cpp
    src/string_key_comparable.cpp
    src/substr_utils.cpp
    src/unescape.cpp
)
target_include_directories(strutillib PUBLIC include)
