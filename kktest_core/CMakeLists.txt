add_library(kktest_interface INTERFACE)
target_include_directories(kktest_interface INTERFACE include)
target_link_libraries(kktest_interface INTERFACE messaging strutil easyflags)

add_library(kktest_impl OBJECT
    src/definers.cpp
    src/driver.cpp
    src/errors.cpp
    src/executor.cpp
    src/group.cpp
        src/test_case_registry.cpp
    src/test.cpp
    src/box_executor.cpp
    src/test_container.cpp
    src/main.cpp
    src/plugin_api_impl.cpp
)
target_compile_options(kktest_impl PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(kktest_impl kktest_interface)

# Examples
function(example FILE_NAME)
    add_executable("example_${FILE_NAME}" examples/${FILE_NAME}.cpp)
    target_link_libraries("example_${FILE_NAME}" kktest)
endfunction()

example(basic)
example(set_up_tear_down)
example(group)
