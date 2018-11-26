cmake_minimum_required(VERSION 3.7)
project(kktest_library)
set(CMAKE_CXX_STANDARD 11)

add_library(kktest SHARED
    src/core/definers.cpp
    src/core/driver.cpp
    src/core/errors.cpp
    src/core/executor.cpp
    src/core/filename.cpp
    src/core/group.cpp
    src/core/plugin.cpp
    src/core/test_case_registry.cpp
    src/core/test.cpp

    src/plugins/logging_plugin.cpp
    src/plugins/pipe_plugin.cpp
    src/plugins/box_plugin/box_executor.cpp
    src/plugins/box_plugin/box_plugin.cpp
    src/plugins/box_plugin/test_container.cpp

    src/main.cpp
)
target_compile_options(kktest PRIVATE -Wall -Wextra -pedantic)
target_include_directories(kktest PUBLIC include PRIVATE src)
target_link_libraries(kktest messaging strutil fsystem logging easyflags)

# Examples
function(example FILE_NAME)
    add_executable("example_${FILE_NAME}" examples/${FILE_NAME}.cpp)
    target_link_libraries("example_${FILE_NAME}" kktest)
endfunction()

example(basic)
example(set_up_tear_down)
example(group)
