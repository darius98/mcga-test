#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_CONFIG_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_CONFIG_HPP_

#include <kktest_impl/types.hpp>

namespace kktest {

struct TestConfig {
    String description = "-";
    bool optional = false;
    double timeTicksLimit = 1.0;
};

struct GroupConfig {
    String description = "-";
};

}  // namespace kktest

#define testConfig(...) ([&] {kktest::TestConfig _; __VA_ARGS__; return _; }())
#define groupConfig(...) ([&] {kktest::GroupConfig _; __VA_ARGS__; return _; }())

#endif  // KKTEST_CORE_INCLUDE_KKTEST_IMPL_CONFIG_HPP_
