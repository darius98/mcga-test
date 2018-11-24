cmake_minimum_required(VERSION 3.7)
project(KKTestRunner VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

set(SRC_FOLDER src)
add_executable(runner
    src/explorer/explorer.cpp

    src/test_execution_loop/kktest_lib_info.cpp
    src/test_execution_loop/test_execution_cycle.cpp
    src/test_execution_loop/test_execution_loop.cpp

    src/entry_point.cpp
)
target_include_directories(runner PRIVATE ${SRC_FOLDER})
target_link_libraries(runner easyflags messaging fsystem logging)
