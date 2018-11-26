cmake_minimum_required(VERSION 3.7)
project(kktest_extension_feedback)

add_library(kktest_extension_feedback_test_logger
    src/test_logger.cpp
)
target_include_directories(kktest_extension_feedback_test_logger PUBLIC include)
target_link_libraries(kktest_extension_feedback_test_logger kktest_interface)

add_library(kktest_extension_feedback OBJECT
    src/logging_plugin.cpp
    src/pipe_plugin.cpp
)
target_link_libraries(kktest_extension_feedback kktest_extension_feedback_test_logger)
