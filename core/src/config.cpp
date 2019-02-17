#include "core/include/kktest_impl/config.hpp"

using namespace std;

namespace kktest {

TestConfig::TestConfig() = default;
TestConfig::TestConfig(string _description): description(move(_description)) {}
TestConfig::TestConfig(const char* _description): description(_description) {}

GroupConfig::GroupConfig() = default;
GroupConfig::GroupConfig(string _description): description(move(_description)) {
}
GroupConfig::GroupConfig(const char* _description): description(_description) {}

}
