add_library(kktest_extension_matchers INTERFACE)
target_include_directories(kktest_extension_matchers INTERFACE include)
target_link_libraries(kktest_extension_matchers INTERFACE kktest_core)

add_kktest(kktest_extension_matchers_test
    tests/main.cpp
    tests/comparison_matchers_test.cpp
    tests/composite_matchers_test.cpp
    tests/functional_matchers_test.cpp
    tests/iterable_matchers_test.cpp
    tests/numeric_matchers_test.cpp
    tests/pointer_matchers_test.cpp
    tests/string_matchers_test.cpp
    tests/truth_matchers_test.cpp
)

install(DIRECTORY include DESTINATION .)
