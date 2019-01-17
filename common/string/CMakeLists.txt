add_library(kktest_string
    src/string.cpp
)
target_include_directories(kktest_string PUBLIC include)
install(DIRECTORY include DESTINATION .)

add_kktest(kktest_string_test
    tests/string_test.cpp
)
