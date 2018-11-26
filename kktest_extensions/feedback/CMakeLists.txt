cmake_minimum_required(VERSION 3.7)
project(kktest_extension_feedback)

add_library(kktest_extension_feedback OBJECT
    src/logging_plugin.cpp
    src/pipe_plugin.cpp
)
target_link_libraries(kktest_extension_feedback kktest_interface logging)
