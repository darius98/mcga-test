set(SRC_FOLDER src)
add_executable(kktest_test_runner
    src/explorer/explorer.cpp
    src/test_execution_loop/test_execution_cycle.cpp
    src/test_execution_loop/test_execution_loop.cpp
    src/entry_point.cpp
)
target_include_directories(kktest_test_runner PRIVATE ${SRC_FOLDER})
target_link_libraries(kktest_test_runner kktest_extension_feedback_test_logger fsystem)
