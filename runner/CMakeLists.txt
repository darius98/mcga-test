cmake_minimum_required(VERSION 3.7)
project(KKTestRunner VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 11)

set(SRC_FOLDER src)
file(GLOB UTILS ${SRC_FOLDER}/utils/*.hpp ${SRC_FOLDER}/utils/*.cpp)
file(GLOB OS ${SRC_FOLDER}/os/*.hpp ${SRC_FOLDER}/os/*.cpp)
file(GLOB EXPLORER ${SRC_FOLDER}/explorer/*.hpp ${SRC_FOLDER}/explorer/*.cpp)
file(GLOB TEST_EXECUTION_LOOP ${SRC_FOLDER}/test_execution_loop/*.hpp ${SRC_FOLDER}/test_execution_loop/*.cpp)
add_executable(runner
        ${SRC_FOLDER}/entry_point.cpp
        ${UTILS}
        ${OS}
        ${EXPLORER}
        ${TEST_EXECUTION_LOOP}
    )
target_include_directories(runner PRIVATE ${SRC_FOLDER})
target_link_libraries(runner easyflags messaginglib fsystemlib)
