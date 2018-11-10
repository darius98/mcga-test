#ifndef KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_CONFIG_H_
#define KKTEST_INCLUDE_KKTEST_PUBLIC_CORE_CONFIG_H_

#include <string>


namespace kktest {

struct TestConfig {
    std::string file = "";
    int line = -1;
    std::string description = "-";
    bool optional = false;
    double timeTicksLimit = 1.0;
};

struct GroupConfig {
    std::string file = "";
    int line = -1;
    std::string description = "-";
};

}

#define testConfig(...) ([&]{kktest::TestConfig $;__VA_ARGS__; return $;}())
#define groupConfig(...) ([&]{kktest::GroupConfig $;__VA_ARGS__; return $;}())

#endif
