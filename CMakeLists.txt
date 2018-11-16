cmake_minimum_required(VERSION 3.7)
project(KKTest)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(BUILD_TYPE "DEV" CACHE STRING "BUILD_TYPE")

# Third party targets
set(THIRD_PARTY_DIR third_party)
add_subdirectory("${THIRD_PARTY_DIR}/autojson")
add_subdirectory("${THIRD_PARTY_DIR}/easyflags")

set(BIN_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")
set(LIB_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(TESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/tests")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

set(KKTEST_LIB_DIR lib)
set(KKTEST_LIB_SRC_DIR ${KKTEST_DIR}/src)
set(THIRD_PARTY_DIR third_party)

add_subdirectory(common)
add_subdirectory(lib)
add_subdirectory(runner)
