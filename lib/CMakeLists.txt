cmake_minimum_required(VERSION 3.8.2)
project(kktest_library)
set(CMAKE_CXX_STANDARD 17)

file(GLOB KKTestLibImpl
        include/kktest
        include/kktest_matchers
        include/*.hpp
        include/*/*.hpp
        include/*/*/*.hpp
        src/*.cpp
        src/*.hpp
        src/*/*.cpp
        src/*/*.hpp
        src/*/*/*.cpp
        src/*/*/*.hpp
    )
add_library(kktest SHARED ${KKTestLibImpl})
target_compile_options(kktest PRIVATE -Wall -Wextra -pedantic)
target_include_directories(kktest PUBLIC include PRIVATE src PRIVATE include/kktest_public)
target_link_libraries(kktest messaging strutil fsystem logging easyflags)

if (BUILD_TYPE STREQUAL "DEV")
    add_subdirectory(examples)
    add_subdirectory(tests)
endif ()
