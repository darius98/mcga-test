cmake_minimum_required(VERSION 3.7)

GET_PROPERTY(__LOCAL_KKTEST_LIBRARY_INCLUDE_GUARD__ GLOBAL PROPERTY __KKTEST_LIBRARY_INCLUDE_GUARD__)
if(__LOCAL_KKTEST_LIBRARY_INCLUDE_GUARD__)
    return()
endif()
SET_PROPERTY(GLOBAL PROPERTY __KKTEST_LIBRARY_INCLUDE_GUARD__ TRUE)

if (NOT TARGET kktest)
    find_library(__KKTEST_AUTOJSON_LIB autojson)
    find_library(__KKTEST_EASYFLAGS_LIB easyflags)
    find_library(__KKTEST_KKTEST_LIB kktest)
endif()

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
    target_link_libraries(${TEST_NAME} ${LIB_LIST})
    if (TARGET kktest)
        target_link_libraries(${TEST_NAME} kktest)
    else()
        # TODO(darius98): This is a big hack.
        # Try to bundle all 3 libraries into one inside installation script.
        target_link_libraries(${TEST_NAME} ${__KKTEST_KKTEST_LIB}
                                           ${__KKTEST_EASYFLAGS_LIB}
                                           ${__KKTEST_AUTOJSON_LIB})
    endif()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()
