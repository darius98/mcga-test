cmake_minimum_required(VERSION 3.7)

GET_PROPERTY(__LOCAL_KKTEST_LIBRARY_INCLUDE_GUARD__ GLOBAL PROPERTY __KKTEST_LIBRARY_INCLUDE_GUARD__)
if(__LOCAL_KKTEST_LIBRARY_INCLUDE_GUARD__)
    return()
endif()
SET_PROPERTY(GLOBAL PROPERTY __KKTEST_LIBRARY_INCLUDE_GUARD__ TRUE)

# Third party software: autojson
set(AUTOJSON_DIR "${KKTEST_THIRD_PARTY_DIR}/autojson")
file(GLOB AUTOJSON_SRC ${AUTOJSON_DIR}/src/*.cpp)
add_library(autojson ${AUTOJSON_SRC})
target_compile_definitions(autojson PUBLIC autojsonuselib)
target_include_directories(autojson PUBLIC "${AUTOJSON_DIR}/include")

# Third party software: easyflags
set(EASYFLAGS_DIR "${KKTEST_THIRD_PARTY_DIR}/easyflags")
add_library(easyflags "${EASYFLAGS_DIR}/src/EasyFlags.cpp")
target_compile_definitions(easyflags PUBLIC easyflagsuselib)
target_include_directories(easyflags PUBLIC "${EASYFLAGS_DIR}/include")
target_link_libraries(easyflags autojson)

# Set KKTest library source directories
# Allow them to be set externally for dev purposes
if(NOT KKTEST_DIR)
    set(KKTEST_DIR "${CMAKE_CURRENT_LIST_DIR}/kktest_src")
endif()
if(NOT KKTEST_SRC_DIR)
    set(KKTEST_SRC_DIR "${KKTEST_DIR}/src")
endif()
if(NOT KKTEST_THIRD_PARTY_DIR)
    set(KKTEST_THIRD_PARTY_DIR "${KKTEST_DIR}/third_party")
endif()

file(GLOB KKTestLibImpl ${KKTEST_SRC_DIR}/testing/*.cpp
        ${KKTEST_SRC_DIR}/testing/*.hpp
        ${KKTEST_SRC_DIR}/utils/filename.hpp
        ${KKTEST_SRC_DIR}/kktest)
file(GLOB KKTestLibImpl ${KKTEST_SRC_DIR}/testing/*.cpp)
add_library(kktest ${KKTestLibImpl})
target_include_directories(kktest PUBLIC ${KKTEST_SRC_DIR})
target_link_libraries(kktest autojson easyflags)

file(GLOB KKTestMatchersLibImpl ${KKTEST_SRC_DIR}/matcher/*.cpp
                                ${KKTEST_SRC_DIR}/matcher/*.hpp
                                ${KKTEST_SRC_DIR}/utils/filename.hpp
                                ${KKTEST_SRC_DIR}/utils/streamer.hpp
                                ${KKTEST_SRC_DIR}/kktest_matchers)
add_library(kktest_matchers ${KKTestMatchersLibImpl})
target_include_directories(kktest_matchers PUBLIC ${KKTEST_SRC_DIR})
target_link_libraries(kktest_matchers kktest)

function(testcase TEST_NAME FILE_NAME)
    if (NOT TESTS_PATH)
        message(FATAL_ERROR "TESTS_PATH variable not set.")
    endif ()

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
