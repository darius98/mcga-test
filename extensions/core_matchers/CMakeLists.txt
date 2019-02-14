add_library(kktest_extension_core_matchers
    src/comparison_matchers.cpp
    src/functional_matchers.cpp
    src/iterable_matchers.cpp
    src/matcher.cpp
    src/numeric_matchers.cpp
    src/pointer_matchers.cpp
    src/string_matchers.cpp
    src/truth_matchers.cpp
)
target_include_directories(kktest_extension_core_matchers PUBLIC include)
target_link_libraries(kktest_extension_core_matchers PUBLIC kktest_core)

add_kktest(kktest_extension_core_matchers_test
    tests/comparison_matcher_test.cpp
    tests/composite_matchers_test.cpp
    tests/functional_matchers_test.cpp
    tests/iterable_matchers_test.cpp
    tests/numeric_matchers_test.cpp
    tests/pointer_matchers_test.cpp
    tests/string_matchers_test.cpp
    tests/truth_matchers_test.cpp
)

install(DIRECTORY include DESTINATION .)
