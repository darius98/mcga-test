add_library(common_utils
    src/time.cpp
)

add_kktest(common_utils_test
    tests/time_test.cpp
)
