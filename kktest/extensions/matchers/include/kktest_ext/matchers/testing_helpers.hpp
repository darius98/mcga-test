#pragma once

#include <stdexcept>

#include <kktest.hpp>
#include <kktest_ext/matchers/matcher.hpp>

// Helpers for testing the matchers themselves.

#define EXPECT_MATCHER_FAILS(...)                                              \
    try {                                                                      \
        kktest::matchers::expect(__VA_ARGS__);                                 \
        fail("expect(" #__VA_ARGS__ ") did not fail!");                        \
    } catch (const std::exception&) {                                          \
    }

#define EXPECT_MATCHER_MATCHES(...)                                            \
    try {                                                                      \
        kktest::matchers::expect(__VA_ARGS__);                                 \
    } catch (const std::exception&) {                                          \
        fail("expect(" #__VA_ARGS__ ") failed!");                              \
    }
