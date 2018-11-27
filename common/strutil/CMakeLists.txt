add_library(strutil
    src/casing.cpp
    src/string_key_comparable.cpp
    src/substr_utils.cpp
    src/unescape.cpp
)
target_include_directories(strutil PUBLIC include)
