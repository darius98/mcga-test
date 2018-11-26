cmake_minimum_required(VERSION 3.7)
project(kktest_extension_feedback)

# TODO(darius98): Make this not need to be shared!
add_library(kktest_extension_feedback SHARED
    src/logging_plugin.cpp
    src/pipe_plugin.cpp
)
target_link_libraries(kktest_extension_feedback kktest_core logging)
