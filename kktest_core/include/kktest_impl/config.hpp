#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_CONFIG_H_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_CONFIG_H_

#include <string>


namespace kktest {

struct TestConfig {
    std::string description = "-";
    bool optional = false;
    double timeTicksLimit = 1.0;
};

struct GroupConfig {
    std::string description = "-";
};

}

#define testConfig(...) ([&]{kktest::TestConfig _;__VA_ARGS__; return _;}())
#define groupConfig(...) ([&]{kktest::GroupConfig _;__VA_ARGS__; return _;}())

#endif
