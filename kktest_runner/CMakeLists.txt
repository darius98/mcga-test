cmake_minimum_required(VERSION 3.7)
project(KKTestRunner)
set(CMAKE_CXX_STANDARD 14)

set(BIN_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/../build/bin")
set(LIB_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/../build/lib")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

set(KKTEST_DIR ../kktest_lib)
set(KKTEST_SRC_DIR ../kktest_lib/src)
set(KKTEST_THIRD_PARTY_DIR ../third_party)

if(DEV)
    include("../kktest_lib/kktest_library.cmake")
else()
    include("/usr/local/include/kktest.cmake")
endif()

add_executable(kktest_runner entry_point.cpp)
target_link_libraries(kktest_runner autojson easyflags)
