add_library(kktest_interface INTERFACE)
target_include_directories(kktest_interface INTERFACE include)
target_link_libraries(kktest_interface INTERFACE interproc strutil)

add_library(kktest_command_line_arguments
    src/arguments_api.cpp
)
target_link_libraries(kktest_command_line_arguments strutil kktest_interface)

add_library(kktest_impl OBJECT
    src/box_executor.cpp
    src/definers.cpp
    src/driver.cpp
    src/errors.cpp
    src/executor.cpp
    src/extension_api_impl.cpp
    src/group.cpp
    src/main.cpp
    src/test.cpp
    src/test_case_registry.cpp
    src/test_container.cpp
)
target_compile_options(kktest_impl PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(kktest_impl kktest_interface kktest_command_line_arguments)

# Examples
function(example FILE_NAME)
    add_executable("example_${FILE_NAME}" examples/${FILE_NAME}.cpp)
    target_link_libraries("example_${FILE_NAME}" kktest)
endfunction()

example(basic)
example(set_up_tear_down)
example(group)
