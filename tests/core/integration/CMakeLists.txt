include(FindThreads)

add_integration_test(core_single_test_pass
        SCRIPT single_test_pass.py
        SOURCES single_test_pass.cpp
        )

add_integration_test(core_single_test_fail
        SCRIPT single_test_fail.py
        SOURCES single_test_fail.cpp
        )

add_integration_test(core_single_test_optional_fail
        SCRIPT single_test_optional_fail.py
        SOURCES single_test_optional_fail.cpp
        )

add_integration_test(core_single_test_multiple_executions_pass
        SCRIPT single_test_multiple_executions_pass.py
        SOURCES single_test_multiple_executions_pass.cpp
        )

add_integration_test(core_single_test_multiple_executions_fail
        SCRIPT single_test_multiple_executions_fail.py
        SOURCES single_test_multiple_executions_fail.cpp
        )

add_integration_test(core_single_test_kbs NO_EXECUTOR_TYPE
        SCRIPT single_test_kbs.py
        SOURCES single_test_kbs.cpp
        )

add_integration_test(core_single_test_timeout NO_EXECUTOR_TYPE
        SCRIPT single_test_timeout.py
        SOURCES single_test_timeout.cpp
        )

add_integration_test(core_single_test_exit_0 NO_EXECUTOR_TYPE
        SCRIPT single_test_exit_0.py
        SOURCES single_test_exit_0.cpp
        )

add_integration_test(core_single_test_exit_1 NO_EXECUTOR_TYPE
        SCRIPT single_test_exit_1.py
        SOURCES single_test_exit_1.cpp
        )

add_integration_test(core_single_test_fail_thread
        SCRIPT single_test_fail_thread.py
        SOURCES single_test_fail_thread.cpp
        )
target_link_libraries(mcga_test_integration_core_single_test_fail_thread_binary ${CMAKE_THREAD_LIBS_INIT})

add_integration_test(core_single_file_multiple_cases
        SCRIPT single_file_multiple_cases.py
        SOURCES single_file_multiple_cases.cpp
        )

add_integration_test(core_multiple_files
        SCRIPT multiple_files.py
        SOURCES multiple_files_1.cpp multiple_files_2.cpp
        )

add_integration_test(core_multiple_tests_one_fails
        SCRIPT multiple_tests_one_fails.py
        SOURCES multiple_tests_one_fails.cpp
        )

add_integration_test(core_emit_warnings
        SCRIPT emit_warnings.py
        SOURCES emit_warnings.cpp
        )
target_link_libraries(mcga_test_integration_core_emit_warnings_binary ${CMAKE_THREAD_LIBS_INIT})
