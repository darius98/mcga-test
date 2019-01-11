add_library(arguments
    src/arguments_api.cpp
    src/arguments_api_impl.cpp
    src/argument_impl.cpp
)
target_include_directories(arguments PUBLIC include)
target_link_libraries(arguments PUBLIC kktest_string)

add_kktest(arguments_test tests/arguments_test.cpp)
