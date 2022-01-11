add_unit_test(extension_death NO_EXECUTOR_TYPE
        death/check_death_test.cpp
        death/matchers_test.cpp)

add_unit_test(extension_mock
        mock/function_mock_test.cpp)
target_link_libraries(mcga_test_unit_extension_mock mcga_mock)
