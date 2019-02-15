add_library(common_utils_time src/time.cpp)
target_include_directories(common_utils_time PUBLIC include)

add_kktest(common_utils_test
    tests/time_test.cpp
)
