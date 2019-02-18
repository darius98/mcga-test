#include "core/include/kktest_impl/definers.hpp"

#include <cstring>

#include <iostream>

#include "core/src/driver.hpp"
#include "core/src/test_case_registry.hpp"

using namespace std;

namespace kktest {

TestCaseDefiner::TestCaseDefiner(TestCase testCase, const char* name) noexcept {
    TestCaseRegistry::add(testCase, name);
}

void test(TestConfig&& config, Executable func) {
    Driver::getInstance()->addTest(move(config), func);
}

void group(GroupConfig&& config, Executable func) {
    Driver::getInstance()->addGroup(move(config), func);
}

void setUp(Executable func) {
    Driver::getInstance()->addSetUp(func);
}

void tearDown(Executable func) {
    Driver::getInstance()->addTearDown(func);
}

void expect(bool exprResult, const string& failMessage) {
    if (!exprResult) {
        fail(failMessage);
    }
}

void fail(const string& message) {
    if (message.empty()) {
        throw ExpectationFailed("kktest::fail: Test failed.");
    } else {
        throw ExpectationFailed(message);
    }
}

}
