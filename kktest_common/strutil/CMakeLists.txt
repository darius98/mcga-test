add_library(strutil
    src/c_string.cpp
    src/kktest_string.cpp
)
target_include_directories(strutil PUBLIC include)

add_kktest(strutil_test
    tests/c_string_test.cpp
    tests/kktest_string_test.cpp
)
