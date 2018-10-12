cmake_minimum_required(VERSION 3.7)
project(KKTest)
set(CMAKE_CXX_STANDARD 14)

set(BIN_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")
set(LIB_PATH   "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(TESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_PATH})

include("kktest_library.cmake")

function(wiki_example FILE_NAME)
    testcase(wiki_example_${FILE_NAME} wiki_examples/${FILE_NAME}.cpp ${ARGN})
endfunction()

matcher_testcase(StdVectorTests concept/std_vector_test.cpp)

wiki_example(base)
wiki_example(set_up_tear_down_bad)
wiki_example(set_up_tear_down_good)
wiki_example(group)
