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

if (MCGA_test_tests)
    find_package(Python3 3.7 REQUIRED COMPONENTS Interpreter)
endif ()

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
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/string.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/time_tick.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/warning.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/exit_code.cpp
        )

add_library(mcga_test_interface INTERFACE)
target_include_directories(mcga_test_interface INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(mcga_test_obj OBJECT
        ${mcga_test_core_src}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/filter.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/test_description.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/text_stream.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hosted/box_executor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hosted/death.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hosted/entry_point.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hosted/mock.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/hosted/temp.cpp)
target_link_libraries(mcga_test_obj
        PUBLIC mcga_cli
        PUBLIC mcga_proc
        PUBLIC mcga_test_interface)
target_include_directories(mcga_test_obj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(mcga_test_obj
        PUBLIC MCGA_TEST_PRE_ALLOCATED_GROUPS=${MCGA_TEST_PRE_ALLOCATED_GROUPS}
        PUBLIC MCGA_TEST_PRE_ALLOCATED_CALLBACKS=${MCGA_TEST_PRE_ALLOCATED_CALLBACKS})
if (SANITIZER_COMPILE_OPTIONS)
    target_compile_options(mcga_test_obj PUBLIC ${SANITIZER_COMPILE_OPTIONS})
endif ()
if (SANITIZER_LINK_OPTIONS)
    target_link_options(mcga_test_obj PUBLIC ${SANITIZER_LINK_OPTIONS})
endif ()

add_library(mcga_test SHARED $<TARGET_OBJECTS:mcga_test_obj>)
target_link_libraries(mcga_test PRIVATE mcga_test_obj PUBLIC mcga_test_interface)
install(TARGETS mcga_test DESTINATION lib)

add_library(mcga_test_static STATIC $<TARGET_OBJECTS:mcga_test_obj>)
target_link_libraries(mcga_test_static PRIVATE mcga_test_obj PUBLIC mcga_test_interface)
install(TARGETS mcga_test_static DESTINATION lib)

add_library(mcga_test_freestanding_obj OBJECT
        ${mcga_test_core_src}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/freestanding/mem.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/freestanding/ssp.cpp)
target_include_directories(mcga_test_freestanding_obj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_definitions(mcga_test_freestanding_obj
        PUBLIC MCGA_TEST_PRE_ALLOCATED_GROUPS=${MCGA_TEST_PRE_ALLOCATED_GROUPS}
        PUBLIC MCGA_TEST_PRE_ALLOCATED_CALLBACKS=${MCGA_TEST_PRE_ALLOCATED_CALLBACKS}
        PUBLIC MCGA_TEST_THREADING=0
        PUBLIC MCGA_TEST_TIMING=0)
target_link_libraries(mcga_test_freestanding_obj PUBLIC mcga_test_interface)
target_compile_options(mcga_test_freestanding_obj
        PUBLIC -ffreestanding
        PUBLIC -fno-exceptions
        PUBLIC -fno-rtti
        PUBLIC -nostdlib)
target_link_options(mcga_test_freestanding_obj
        PUBLIC -ffreestanding
        PUBLIC -fno-exceptions
        PUBLIC -fno-rtti
        PUBLIC -nostdlib
        PUBLIC -static)

add_library(mcga_test_freestanding_static STATIC $<TARGET_OBJECTS:mcga_test_freestanding_obj>)
target_link_libraries(mcga_test_freestanding_static PRIVATE mcga_test_freestanding_obj PUBLIC mcga_test_interface)
install(TARGETS mcga_test_freestanding_static DESTINATION lib)

if (NOT APPLE)
    add_library(mcga_test_freestanding SHARED $<TARGET_OBJECTS:mcga_test_freestanding_obj>)
    target_link_libraries(mcga_test_freestanding PRIVATE mcga_test_freestanding_obj PUBLIC mcga_test_interface)
    install(TARGETS mcga_test_freestanding DESTINATION lib)
endif ()

install(DIRECTORY include DESTINATION .)

if (MCGA_test_tests)
    enable_testing()
    add_subdirectory(tests)
endif ()
