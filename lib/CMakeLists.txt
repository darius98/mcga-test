cmake_minimum_required(VERSION 3.8.2)
project(kktest_library)
set(CMAKE_CXX_STANDARD 17)

file(GLOB KKTestLibImpl
        src/core/definers.cpp
        src/core/driver.cpp
        src/core/errors.cpp
        src/core/executor.cpp
        src/core/filename.cpp
        src/core/group.cpp
        src/core/plugin.cpp
        src/core/test_case_registry.cpp
        src/core/test.cpp

        src/matchers/functional_matchers.cpp
        src/matchers/iterable_matchers.cpp
        src/matchers/numeric_matchers.cpp
        src/matchers/pointer_matchers.cpp
        src/matchers/string_matchers.cpp
        src/matchers/truth_matcher.cpp

        src/plugins/logging_plugin.cpp
        src/plugins/pipe_plugin.cpp
        src/plugins/box_plugin/box_executor.cpp
        src/plugins/box_plugin/box_plugin.cpp
        src/plugins/box_plugin/test_container.cpp

        src/main.cpp
    )
add_library(kktest SHARED ${KKTestLibImpl})
target_compile_options(kktest PRIVATE -Wall -Wextra -pedantic)
target_include_directories(kktest PUBLIC include PRIVATE src PRIVATE include/kktest_public)
target_link_libraries(kktest messaging strutil fsystem logging easyflags)

if (BUILD_TYPE STREQUAL "DEV")
    add_subdirectory(examples)
    add_subdirectory(tests)
endif ()
