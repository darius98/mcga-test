#ifndef KKTEST_CORE_KKTEST_IMPL_CONFIG_HPP_
#define KKTEST_CORE_KKTEST_IMPL_CONFIG_HPP_

#include <kktest_common/string.hpp>

namespace kktest {

struct TestConfig {
    String description = "-";
    bool optional = false;
    double timeTicksLimit = 1.0;
};

struct GroupConfig {
    String description = "-";
};

}

#define testConfig(...) \
            ([&] {kktest::TestConfig _; __VA_ARGS__; return _; }())
#define groupConfig(...) \
            ([&] {kktest::GroupConfig _; __VA_ARGS__; return _; }())

#endif
