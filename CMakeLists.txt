cmake_minimum_required(VERSION 3.18)
project(MCGA_test)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

option(MCGA_test_tests "Build MCGA Test tests" OFF)

if (NOT TARGET mcga_cli)
    include(FetchContent)
    FetchContent_Declare(mcga_cli URL https://github.com/darius98/mcga_cli/archive/refs/heads/master.zip)
    FetchContent_MakeAvailable(mcga_cli)
endif ()

add_library(mcga_test_obj OBJECT
        ${CMAKE_CURRENT_SOURCE_DIR}/src/entry_point.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/box_executor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/driver.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/executor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/group.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/mcga_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/scan_executor.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/time_tick.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/core/warning.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/death/check_death.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/feedback/color.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/feedback/ext.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/feedback/test_logger.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/filter/ext.cpp

        ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/mock/base.cpp)
target_link_libraries(mcga_test_obj PRIVATE mcga_cli)
target_include_directories(mcga_test_obj
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(mcga_test SHARED $<TARGET_OBJECTS:mcga_test_obj>)
target_link_libraries(mcga_test PRIVATE mcga_cli PUBLIC mcga_test_obj)

add_library(mcga_test_static STATIC $<TARGET_OBJECTS:mcga_test_obj>)
target_link_libraries(mcga_test_static PRIVATE mcga_cli PUBLIC mcga_test_obj)

add_library(mcga_mock STATIC ${CMAKE_CURRENT_SOURCE_DIR}/src/extensions/mock/libc.cpp)
target_include_directories(mcga_mock
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

install(TARGETS mcga_test DESTINATION lib)
install(TARGETS mcga_test_static DESTINATION lib)
install(DIRECTORY include DESTINATION .)

if (MCGA_test_tests)
    enable_testing()
    add_subdirectory(tests)
endif ()
