add_library(common_utils
    src/time.cpp
    src/string.cpp
)

add_kktest(common_utils_test
    tests/string_test.cpp
    tests/time_test.cpp
)
