cmake_minimum_required(VERSION 3.7)
project(KKTest)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

set(BUILD_TYPE "DEV" CACHE STRING "BUILD_TYPE")

set(BIN_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")
set(LIB_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

set(KKTEST_LIB_DIR lib)
set(KKTEST_LIB_SRC_DIR ${KKTEST_DIR}/src)
set(THIRD_PARTY_DIR third_party)

# Third party software: autojson
set(AUTOJSON_DIR "${THIRD_PARTY_DIR}/autojson")
file(GLOB AUTOJSON_SRC ${AUTOJSON_DIR}/src/*.cpp)
add_library(autojson ${AUTOJSON_SRC})
target_compile_definitions(autojson PUBLIC autojsonuselib)
target_include_directories(autojson PUBLIC "${AUTOJSON_DIR}/include")

# Third party software: easyflags
set(EASYFLAGS_DIR "${THIRD_PARTY_DIR}/easyflags")
add_library(easyflags "${EASYFLAGS_DIR}/src/EasyFlags.cpp")
target_compile_definitions(easyflags PUBLIC easyflagsuselib)
target_include_directories(easyflags PUBLIC "${EASYFLAGS_DIR}/include")
target_link_libraries(easyflags autojson)

add_subdirectory(common)
add_subdirectory(lib)
add_subdirectory(runner)
