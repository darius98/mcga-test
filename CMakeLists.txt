cmake_minimum_required(VERSION 3.12)
project(KKTest)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(BUILD_TYPE "DEV" CACHE STRING "BUILD_TYPE")

# Third party targets
add_subdirectory(third_party/autojson)
add_subdirectory(third_party/easyflags)

set(TESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/tests")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/arc")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

include(kktest.cmake)

add_subdirectory(common/strutil)
add_subdirectory(common/fsystem)
add_subdirectory(common/messaging)

add_subdirectory(kktest_core)

include(build_kktest_shared_lib.cmake)

add_subdirectory(kktest_runner)
