add_library(common_string src/string.cpp)
target_include_directories(common_string PUBLIC include)
install(DIRECTORY include DESTINATION .)

add_kktest(string_test
    tests/string_test.cpp
)
