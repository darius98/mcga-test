cmake_minimum_required(VERSION 3.7)

if (BUILD_TYPE STREQUAL "DEV")
    set(KKTEST_DEV_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
endif ()

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
    install(TARGETS ${TEST_NAME} DESTINATION ${TEST_NAME})
    target_link_libraries(${TEST_NAME} kktest)
    if (BUILD_TYPE STREQUAL "DEV")
        target_include_directories(${TEST_NAME} PRIVATE ${KKTEST_DEV_INCLUDE_DIR})
    endif()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()
