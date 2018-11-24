cmake_minimum_required(VERSION 3.7)

include("../kktest.cmake")

add_kktest(comparison_matcher_test    matcher/comparison_matcher_test.cpp)
add_kktest(composite_matcher_test     matcher/composite_matchers_test.cpp)
add_kktest(functional_matchers_test   matcher/functional_matchers_test.cpp)
add_kktest(iterable_matchers_test     matcher/iterable_matchers_test.cpp)
add_kktest(numeric_matchers_test      matcher/numeric_matchers_test.cpp)
add_kktest(pointer_matchers_test      matcher/pointer_matchers_test.cpp)
add_kktest(string_matchers_test       matcher/string_matchers_test.cpp)
add_kktest(truth_matchers_test        matcher/truth_matchers_test.cpp)
