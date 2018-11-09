#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_TEST_CONFIG_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_TEST_CONFIG_H_

#include <string>


namespace kktest {

struct TestConfig {
    std::string description = "-";
    bool optional = false;
    int timeTicksLimit = 1;
};

}

#define testConfig(...) ([&]{kktest::TestConfig $;__VA_ARGS__; return $;}())

#endif
