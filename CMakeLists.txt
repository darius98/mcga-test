cmake_minimum_required(VERSION 3.7)
project(KKTest)
set(CMAKE_CXX_STANDARD 14)

set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(BIN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})


add_definitions(-Dautojsonuselib)
include_directories(./third_party/autojson/include)
add_subdirectory(third_party/autojson)

add_definitions(-Deasyflagsuselib)
include_directories(./third_party/easyflags/include)
add_subdirectory(third_party/easyflags)

include_directories(./src)

file(GLOB KKTestLibImpl include/kktest src/**/*.hpp src/testing/*.cpp)
add_library("kktest" ${KKTestLibImpl})
target_link_libraries(kktest autojson easyflags)

file(GLOB KKTestMatchersLibImpl include/kktest_matchers src/**/*.hpp
            src/matcher/*.cpp src/utils/*.cpp)
add_library("kktest_matchers" ${KKTestMatchersLibImpl})
target_link_libraries(kktest_matchers kktest)

include_directories(./include)
set(T StdVectorTests)
add_executable(${T} concept/std_vector_test.cpp)
install(TARGETS ${T} DESTINATION ${T})
target_link_libraries(${T} kktest kktest_matchers)

function(wiki_example FILE_NAME)
    set(T wiki_example_${FILE_NAME})
    add_executable(${T} wiki_examples/${FILE_NAME}.cpp)
    install(TARGETS ${T} DESTINATION ${T})
    target_link_libraries(${T} kktest kktest_matchers)
endfunction()

wiki_example(base)
wiki_example(set_up_tear_down_bad)
wiki_example(set_up_tear_down_good)
wiki_example(group)
