cmake_minimum_required(VERSION 3.7)
project(strutillib VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(strutillib
    include/strutil
    include/strutil_public/casing.hpp
    include/strutil_public/string_key_comparable.hpp
    include/strutil_public/substr_utils.hpp
    include/strutil_public/unescape.hpp
    src/casing.cpp
    src/string_key_comparable.cpp
    src/substr_utils.cpp
    src/unescape.cpp
)
target_include_directories(strutillib PUBLIC include)
target_include_directories(strutillib PRIVATE include/strutil_public)
