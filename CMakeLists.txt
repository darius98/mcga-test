cmake_minimum_required(VERSION 3.18)
project(MCGA_test)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

option(MCGA_test_tests "Build MCGA Test tests" OFF)
set(MCGA_TEST_PRE_ALLOCATED_GROUPS 8 CACHE STRING
        "How many groups to statically allocate inside the MCGA test library (default is 8).")
set(MCGA_TEST_PRE_ALLOCATED_CALLBACKS 128 CACHE STRING
        "How many callback to statically allocate inside the MCGA test library (default is 128). Callbacks are used for setUp()s, tearDown()s and cleanup()s.")


if (NOT TARGET mcga_cli)
    include(FetchContent)
    FetchContent_Declare(mcga_cli URL https://github.com/darius98/mcga-cli/archive/refs/heads/master.zip)
    FetchContent_MakeAvailable(mcga_cli)
endif ()
if (NOT TARGET mcga_proc)
    include(FetchContent)
    FetchContent_Declare(mcga_proc URL https://github.com/darius98/mcga-proc/archive/refs/heads/master.zip)
    FetchContent_MakeAvailable(mcga_proc)
endif ()

set(mcga_test_core_src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/driver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/executor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/extension_api.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/group.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/mcga_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/memory.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/time_tick.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/warning.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/feedback/exit_code_extension.cpp
        )

add_library(mcga_test_interface INTERFACE)
target_include_directories(mcga_test_interface
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(mcga_test_interface
        INTERFACE MCGA_TEST_PRE_ALLOCATED_GROUPS=${MCGA_TEST_PRE_ALLOCATED_GROUPS}
        INTERFACE MCGA_TEST_PRE_ALLOCATED_CALLBACKS=${MCGA_TEST_PRE_ALLOCATED_CALLBACKS})


add_library(mcga_test_obj OBJECT
        ${mcga_test_core_src}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/death/check_death.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/feedback/color.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/feedback/socket_streaming_extension.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/feedback/stdout_logging_extension.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/feedback/test_logger.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/filter/ext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hosted/entry_point.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hosted/box_executor.cpp)
target_link_libraries(mcga_test_obj
        PRIVATE mcga_cli
        PRIVATE mcga_proc
        PUBLIC mcga_test_interface)

add_library(mcga_test SHARED $<TARGET_OBJECTS:mcga_test_obj>)
target_link_libraries(mcga_test PRIVATE mcga_test_obj)

add_library(mcga_test_static STATIC $<TARGET_OBJECTS:mcga_test_obj>)
target_link_libraries(mcga_test_static PRIVATE mcga_test_obj)

add_library(mcga_test_freestanding STATIC
        ${mcga_test_core_src}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/freestanding/memcpy.cpp)
target_link_libraries(mcga_test_freestanding PRIVATE mcga_test_interface)
target_compile_definitions(mcga_test_freestanding
        PUBLIC MCGA_TEST_THREADING=0
        PUBLIC MCGA_TEST_TIMING=0)
target_compile_options(mcga_test_freestanding
        PUBLIC -ffreestanding
        PUBLIC -fno-exceptions
        PUBLIC -fno-rtti
        PUBLIC -nostdlib)
target_link_options(mcga_test_freestanding
        PUBLIC -ffreestanding
        PUBLIC -nostdlib
        PUBLIC -static)

add_library(mcga_mock SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/mock/base.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/mock/libc.cpp)
target_link_libraries(mcga_mock PRIVATE mcga_test_interface)

install(TARGETS mcga_test DESTINATION lib)
install(TARGETS mcga_test_static DESTINATION lib)
install(DIRECTORY include DESTINATION .)

if (MCGA_test_tests)
    enable_testing()
    add_subdirectory(tests)
endif ()
