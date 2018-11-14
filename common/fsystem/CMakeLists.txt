cmake_minimum_required(VERSION 3.7)
project(fsystemlib VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(fsystemlib
    include/fsystem
    include/error.hpp
    include/extension.hpp
    include/file.hpp
    include/folder.hpp
    include/path.hpp
    src/error.cpp
    src/extension.cpp
    src/file.cpp
    src/folder.cpp
    src/path.cpp
)
target_include_directories(fsystemlib PUBLIC include)
target_link_libraries(fsystemlib strutillib)
