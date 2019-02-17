add_library(common_string src/string.cpp)

add_kktest(common_string_test
    tests/string_test.cpp
)
