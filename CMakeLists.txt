cmake_minimum_required(VERSION 3.12)
project(KKTest)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(BUILD_TYPE "DEV" CACHE STRING "BUILD_TYPE")

set(TESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/tests")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/arc")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

function(add_kktest TEST_NAME)
    set(PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TESTS_PATH})

    add_executable(${ARGV})
    target_link_libraries(${TEST_NAME} kktest)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()

add_subdirectory(common/strutil)
add_subdirectory(common/fsystem)
add_subdirectory(common/messaging)

add_subdirectory(kktest_core)
add_subdirectory(kktest_manager)

include(generated/build_kktest.cmake)
