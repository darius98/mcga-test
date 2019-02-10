cmake_minimum_required(VERSION 3.7)
project(KKTest)
set(CMAKE_CXX_STANDARD 11)

# TODO(darius98): Find a cleaner way to detect this automatically.
add_definitions(-DKKTEST_USE_ABI_DEMANGLE)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_INSTALL_PREFIX /usr/local/)

set(TESTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/build/tests")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

function(add_kktest TEST_NAME)
    set(PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${TESTS_PATH})

    add_executable(${ARGV})
    target_link_libraries(${TEST_NAME} kktest)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PREV_CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endfunction()

add_subdirectory(common/string)
add_subdirectory(common/interproc)
add_subdirectory(common/utils)

add_subdirectory(core)

add_subdirectory(extensions/core_matchers)
add_subdirectory(extensions/feedback)
add_subdirectory(extensions/test_runner)

add_library(kktest
    entry/entry_point.cpp
    $<TARGET_OBJECTS:cppli_objects>
    $<TARGET_OBJECTS:interproc>
    $<TARGET_OBJECTS:kktest_string>
    $<TARGET_OBJECTS:kktest_time>
    $<TARGET_OBJECTS:kktest_impl>

    $<TARGET_OBJECTS:kktest_extension_core_matchers>
    $<TARGET_OBJECTS:kktest_extension_feedback>
    $<TARGET_OBJECTS:kktest_extension_feedback_test_logger>)

install(TARGETS kktest ARCHIVE DESTINATION lib)

target_link_libraries(kktest kktest_interface
        kktest_extension_core_matchers
        kktest_extension_feedback)

add_subdirectory(third_party/cppli)
