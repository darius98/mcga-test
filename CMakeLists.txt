cmake_minimum_required(VERSION 3.18)
project(MCGA_test)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

option(MCGA_test_tests "Build MCGA Test tests" OFF)

add_subdirectory(third_party/termcolor EXCLUDE_FROM_ALL)

add_library(mcga_test_obj OBJECT
        src/entry_point.cpp

        src/core/box_executor.cpp
        src/core/driver.cpp
        src/core/executor.cpp
        src/core/group.cpp
        src/core/main.cpp
        src/core/mcga_test.cpp
        src/core/scan_executor.cpp
        src/core/time_tick.cpp
        src/core/test.cpp
        src/core/warning.cpp

        src/extensions/death/check_death.cpp

        src/extensions/feedback/ext.cpp
        src/extensions/feedback/test_logger.cpp
        )
target_include_directories(mcga_test_obj PRIVATE src)
target_link_libraries(mcga_test_obj PUBLIC mcga_cli PRIVATE termcolor)

add_library(mcga_test SHARED $<TARGET_OBJECTS:mcga_test_obj>)
target_link_libraries(mcga_test PUBLIC mcga_test_obj)

add_library(mcga_test_static STATIC $<TARGET_OBJECTS:mcga_test_obj>)
target_link_libraries(mcga_test_static PUBLIC mcga_test_obj)

install(TARGETS mcga_test DESTINATION lib)
install(TARGETS mcga_test_static DESTINATION lib)
install(DIRECTORY include DESTINATION .)

if (MCGA_test_tests)
    enable_testing()
    add_subdirectory(tests)
endif ()
