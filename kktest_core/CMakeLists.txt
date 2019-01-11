add_library(kktest_interface INTERFACE)
target_include_directories(kktest_interface INTERFACE include)
target_link_libraries(kktest_interface INTERFACE interproc kktest_string arguments kktest_time)

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
target_link_libraries(kktest_impl kktest_interface)
