add_library(kktest_extension_core_matchers OBJECT
    src/functional_matchers.cpp
    src/iterable_matchers.cpp
    src/numeric_matchers.cpp
    src/pointer_matchers.cpp
    src/string_matchers.cpp
    src/truth_matchers.cpp
)
target_include_directories(kktest_extension_core_matchers PUBLIC include)

add_kktest(comparison_matcher_test    tests/comparison_matcher_test.cpp)
add_kktest(composite_matcher_test     tests/composite_matchers_test.cpp)
add_kktest(functional_matchers_test   tests/functional_matchers_test.cpp)
add_kktest(iterable_matchers_test     tests/iterable_matchers_test.cpp)
add_kktest(numeric_matchers_test      tests/numeric_matchers_test.cpp)
add_kktest(pointer_matchers_test      tests/pointer_matchers_test.cpp)
add_kktest(string_matchers_test       tests/string_matchers_test.cpp)
add_kktest(truth_matchers_test        tests/truth_matchers_test.cpp)
