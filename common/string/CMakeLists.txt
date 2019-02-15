add_library(common_string src/string.cpp)
target_include_directories(common_string PUBLIC include)

add_kktest(common_string_test
    tests/string_test.cpp
)
