#ifndef KKTEST_CORE_INCLUDE_KKTEST_IMPL_CONFIG_HPP_
#define KKTEST_CORE_INCLUDE_KKTEST_IMPL_CONFIG_HPP_

#include <string>

#include <kktest_impl/export.hpp>

namespace kktest {

struct KKTEST_API TestConfig {
    std::string description = "-";
    bool optional = false;
    double timeTicksLimit = 1.0;

    TestConfig();
    TestConfig(std::string _description); // NOLINT(google-explicit-constructor)
    TestConfig(const char* _description); // NOLINT(google-explicit-constructor)

    TestConfig& setDescription(const std::string& _description);
    TestConfig& setOptional(bool _optional);
    TestConfig& setTimeTicksLimit(double _timeTicksLimit);
};

struct KKTEST_API GroupConfig {
    std::string description = "-";

    GroupConfig();
    GroupConfig(std::string _description);// NOLINT(google-explicit-constructor)
    GroupConfig(const char* _description);// NOLINT(google-explicit-constructor)

    GroupConfig& setDescription(const std::string& _description);
};

}

#endif
