add_executable(kktest_manager
    src/entry_point.cpp
)
target_link_libraries(kktest_manager kktest_command_line_arguments)
