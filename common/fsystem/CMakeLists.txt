cmake_minimum_required(VERSION 3.7)
project(FSystem VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(fsystem
    include/fsystem
    include/fsystem_public/error.hpp
    include/fsystem_public/extension.hpp
    include/fsystem_public/file.hpp
    include/fsystem_public/folder.hpp
    include/fsystem_public/path.hpp
    src/error.cpp
    src/extension.cpp
    src/file.cpp
    src/folder.cpp
    src/path.cpp
)
target_include_directories(fsystem PUBLIC include)
target_include_directories(fsystem PRIVATE include/fsystem_public)
target_link_libraries(fsystem strutil)
