cmake_minimum_required(VERSION 3.12.4)
project(KKTest)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

include_directories(.)

add_subdirectory(third_party/cppli EXCLUDE_FROM_ALL)
add_subdirectory(third_party/termcolor EXCLUDE_FROM_ALL)

add_subdirectory(common/interproc)
add_subdirectory(common/utils)
add_subdirectory(kktest)

enable_testing()
add_subdirectory(tests)
