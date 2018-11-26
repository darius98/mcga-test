cmake_minimum_required(VERSION 3.7)
project(kktest_core)
set(CMAKE_CXX_STANDARD 11)

add_library(kktest_core INTERFACE)
target_include_directories(kktest_core INTERFACE include)
target_link_libraries(kktest_core INTERFACE messaging strutil fsystem easyflags)

add_library(kktest SHARED
    src/definers.cpp
    src/driver.cpp
    src/errors.cpp
    src/executor.cpp
    src/filename.cpp
    src/group.cpp
    src/plugin_api.cpp
    src/test_case_registry.cpp
    src/test.cpp
    src/box_executor.cpp
    src/test_container.cpp
    src/main.cpp
)
target_compile_options(kktest PRIVATE -Wall -Wextra -pedantic)
target_include_directories(kktest PRIVATE src)
target_link_libraries(kktest kktest_core)

# Examples
function(example FILE_NAME)
    add_executable("example_${FILE_NAME}" examples/${FILE_NAME}.cpp)
    target_link_libraries("example_${FILE_NAME}" kktest)
endfunction()

example(basic)
example(set_up_tear_down)
example(group)
