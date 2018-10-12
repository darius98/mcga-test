cmake_minimum_required(VERSION 3.7)
project(KKTestLibs)
set(CMAKE_CXX_STANDARD 14)

add_definitions(-Dautojsonuselib)
include_directories(${CMAKE_CURRENT_LIST_DIR}/third_party/autojson/include)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/autojson CMakeFiles_autojson)

add_definitions(-Deasyflagsuselib)
include_directories(${CMAKE_CURRENT_LIST_DIR}/third_party/easyflags/include)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/easyflags CMakeFiles_easyflags)

include_directories(${CMAKE_CURRENT_LIST_DIR}/src)
file(GLOB KKTestLibImpl ${CMAKE_CURRENT_LIST_DIR}/src/testing/*.cpp)
add_library("kktest" ${KKTestLibImpl})
target_link_libraries(kktest autojson easyflags)

file(GLOB KKTestMatchersLibImpl ${CMAKE_CURRENT_LIST_DIR}/src/matcher/*.cpp)
add_library("kktest_matchers" ${KKTestMatchersLibImpl})

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

function(testcase TEST_NAME FILE_NAME)
    if (NOT TESTS_PATH)
        message(FATAL_ERROR "TESTS_PATH variable not set.")
    endif (NOT TESTS_PATH)

    get_property(logged_tests_path GLOBAL PROPERTY KKTEST_LOGGED_TESTS_PATH)
    if (NOT logged_tests_path)
        message(STATUS "KKTEST::TEST_CASE::TESTS_PATH=${TESTS_PATH}")
        set_property(GLOBAL PROPERTY KKTEST_LOGGED_TESTS_PATH TRUE)
    endif (NOT logged_tests_path)

    set(PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TESTS_PATH})
    add_executable(${TEST_NAME} ${FILE_NAME})
    install(TARGETS ${TEST_NAME} DESTINATION ${TEST_NAME})
    target_link_libraries(${TEST_NAME} kktest ${ARGN})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()
function(matcher_testcase TEST_NAME FILE_NAME)
    testcase(${TEST_NAME} ${FILE_NAME} kktest_matchers)
endfunction()
