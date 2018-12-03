add_library(strutil
    src/c_string.cpp
    src/casing.cpp
    src/string_key_comparable.cpp
    src/substr_utils.cpp
    src/unescape.cpp
)
target_include_directories(strutil PUBLIC include)

add_kktest(strutil_test
    tests/c_string_test.cpp
    tests/casing_test.cpp
    tests/substr_utils_test.cpp
)
