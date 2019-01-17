add_library(arguments OBJECT
    src/arguments_api.cpp
    src/arguments_api_impl.cpp
    src/argument_impl.cpp
)
target_include_directories(arguments PUBLIC include)
install(DIRECTORY include DESTINATION include)

add_kktest(arguments_test tests/arguments_test.cpp)
