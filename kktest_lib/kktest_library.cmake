if(__KKTEST_LIBRARY_INCLUDE_GUARD__)
    return()
endif()
set(__KKTEST_LIBRARY_INCLUDE_GUARD__ TRUE)

cmake_minimum_required(VERSION 3.7)
project(KKTestLibs)
set(CMAKE_CXX_STANDARD 14)

add_definitions(-Dautojsonuselib)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../third_party/autojson/include)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../third_party/autojson CMakeFiles_autojson)

add_definitions(-Deasyflagsuselib)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../third_party/easyflags/include)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../third_party/easyflags CMakeFiles_easyflags)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
file(GLOB KKTestLibImpl
        ${CMAKE_CURRENT_LIST_DIR}/src/testing/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/testing/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/src/utils/filename.hpp
        )
add_library("kktest" ${KKTestLibImpl} src/kktest)
target_link_libraries(kktest autojson easyflags)

file(GLOB KKTestMatchersLibImpl
        ${CMAKE_CURRENT_LIST_DIR}/src/matcher/*.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/matcher/*.hpp
        ${CMAKE_CURRENT_LIST_DIR}/src/utils/filename.hpp
        ${CMAKE_CURRENT_LIST_DIR}/src/utils/streamer.hpp
        )
add_library("kktest_matchers" ${KKTestMatchersLibImpl} src/kktest_matchers)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)

function(testcase TEST_NAME FILE_NAME)
    if (NOT TESTS_PATH)
        message(FATAL_ERROR "TESTS_PATH variable not set.")
    endif (NOT TESTS_PATH)

    message(STATUS "KKTEST::TEST_CASE ${TESTS_PATH}/${TEST_NAME}")

    set(PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TESTS_PATH})

    foreach(arg IN LISTS ARGN)
        if (TARGET ${arg})
            list(APPEND LIB_LIST ${arg})
        else()
            list(APPEND FILE_LIST ${arg})
        endif()
    endforeach()

    add_executable(${TEST_NAME} ${FILE_NAME} ${FILE_LIST})
    install(TARGETS ${TEST_NAME} DESTINATION ${TEST_NAME})
    target_link_libraries(${TEST_NAME} kktest ${LIB_LIST})

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()

function(matcher_testcase TEST_NAME FILE_NAME)
    testcase(${TEST_NAME} ${FILE_NAME} kktest_matchers ${ARGN})
endfunction()
