cmake_minimum_required(VERSION 3.7)
project(FSystem VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

add_library(fsystem
    src/error.cpp
    src/extension.cpp
    src/file.cpp
    src/folder.cpp
    src/path.cpp
)
target_include_directories(fsystem PUBLIC include)
target_include_directories(fsystem PRIVATE include/fsystem_public)
target_link_libraries(fsystem strutil)
