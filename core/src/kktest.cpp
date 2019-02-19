#include "core/include/kktest.hpp"

#include "core/src/driver.hpp"
#include "core/src/export.hpp"
#include "core/src/test_case_registry.hpp"

using namespace std;

namespace kktest {

KKTEST_EXPORT TestConfig::TestConfig(string _description):
        description(move(_description)) {}

KKTEST_EXPORT TestConfig::TestConfig(const char* _description):
        description(_description) {}

KKTEST_EXPORT TestConfig& TestConfig::setDescription(string description) {
    this->description = move(description);
    return *this;
}

KKTEST_EXPORT TestConfig& TestConfig::setOptional(bool optional) {
    this->optional = optional;
    return *this;
}

KKTEST_EXPORT TestConfig& TestConfig::setTimeTicksLimit(double timeTicksLimit) {
    this->timeTicksLimit = timeTicksLimit;
    return *this;
}

KKTEST_EXPORT GroupConfig::GroupConfig(string _description):
        description(move(_description)) {}

KKTEST_EXPORT GroupConfig::GroupConfig(const char* _description):
        description(_description) {}

KKTEST_EXPORT GroupConfig& GroupConfig::setDescription(string description) {
    this->description = move(description);
    return *this;
}

KKTEST_EXPORT TestCaseDefiner::TestCaseDefiner(void (*testCase)(),
                                               const char* name) {
    TestCaseRegistry::add(testCase, name);
}

KKTEST_EXPORT void test(TestConfig config, Executable func) {
    Driver::getInstance()->addTest(move(config), func);
}

KKTEST_EXPORT void group(GroupConfig config, Executable func) {
    Driver::getInstance()->addGroup(move(config), func);
}

KKTEST_EXPORT void setUp(Executable func) {
    Driver::getInstance()->addSetUp(func);
}

KKTEST_EXPORT void tearDown(Executable func) {
    Driver::getInstance()->addTearDown(func);
}

KKTEST_EXPORT void expect(bool expr, const string& message) {
    if (!expr) {
        fail(message);
    }
}

KKTEST_EXPORT void fail(const string& message) {
    throw ExpectationFailed(message);
}

}
