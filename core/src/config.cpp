#include "core/include/kktest_impl/config.hpp"

using namespace std;

namespace kktest {

TestConfig::TestConfig() = default;

TestConfig::TestConfig(string _description):
        description(move(_description)) {}

TestConfig::TestConfig(const char* _description):
        description(_description) {}

TestConfig& TestConfig::setDescription(const string& _description) {
    description = _description;
    return *this;
}

TestConfig& TestConfig::setOptional(bool _optional) {
    optional = _optional;
    return *this;
}

TestConfig& TestConfig::setTimeTicksLimit(double _timeTicksLimit) {
    timeTicksLimit = _timeTicksLimit;
    return *this;
}

GroupConfig::GroupConfig() = default;

GroupConfig::GroupConfig(string _description):
        description(move(_description)) {}

GroupConfig::GroupConfig(const char* _description):
        description(_description) {}

GroupConfig& GroupConfig::setDescription(const string& _description) {
    description = _description;
    return *this;
}

}
