cmake_minimum_required(VERSION 3.7)

function(add_kktest TEST_NAME)
    if (NOT TESTS_PATH)
        message(FATAL_ERROR "TESTS_PATH variable not set.")
    endif ()

    if (BUILD_TYPE STREQUAL "DEV")
        set(KKTEST_LIB kktest)
    else ()
        find_library(KKTEST_LIB kktest)
    endif()
    if (NOT KKTEST_LIB)
        message(FATAL_ERROR "kktest library not installed.")
    endif()

    set(PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TESTS_PATH})

    add_executable(${ARGV})
    target_link_libraries(${TEST_NAME} kktest)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()
