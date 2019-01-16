add_library(kktest_time src/time.cpp)
target_include_directories(kktest_time PUBLIC include)


add_kktest(common_utils_test
    tests/time_test.cpp
)