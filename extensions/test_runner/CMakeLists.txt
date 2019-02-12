set(SRC_FOLDER src)
add_executable(kktest_test_runner
    src/explorer/explorer.cpp
    src/test_execution_loop/test_execution_cycle.cpp
    src/test_execution_loop/test_execution_loop.cpp
    src/entry_point.cpp
)
target_include_directories(kktest_test_runner PRIVATE ${SRC_FOLDER})
target_link_libraries(kktest_test_runner
        stdc++fs
        cppli
        common_interproc
        common_string
        common_utils_time
        kktest_extension_feedback
        )
set_property(TARGET kktest_test_runner PROPERTY CXX_STANDARD 17)

install(TARGETS kktest_test_runner RUNTIME DESTINATION kktest_bin)
