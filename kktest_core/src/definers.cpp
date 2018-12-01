#include <cstring>

#include <iostream>

#include <kktest_impl/definers.hpp>
#include <kktest_impl/signature.hpp>
#include "driver.hpp"
#include "errors.hpp"
#include "test_case_registry.hpp"

using std::cout;

namespace kktest {

TestCaseDefiner::TestCaseDefiner(void (*testCase)(), const unsigned char* signature) {
    if (memcmp(signature, kkTestSignature, kkTestSigSize) != 0) {
        cout << "Invalid signature passed to TestCaseDefiner. If you didn't do anything weird when "
                "defining test cases, then this might be a bug, please report.\n";
        exit(1);
    }
    TestCaseRegistry::add(testCase);
}

void test(String description, Executable func) {
    test(testConfig(_.description = description), func);
}

void test(const TestConfig& config, Executable func) {
    TestingDriver::getInstance()->addTest(config, func);
}

void group(String description, Executable func) {
    group(groupConfig(_.description = description), func);
}

void group(const GroupConfig& config, Executable func) {
    TestingDriver::getInstance()->addGroup(config, func);
}

void setUp(Executable func) {
    TestingDriver::getInstance()->addSetUp(func);
}

void tearDown(Executable func) {
    TestingDriver::getInstance()->addTearDown(func);
}

namespace detail {

void throwExpectationFailed(const String& message) {
    if (message.empty()) {
        throw ExpectationFailed("Expectation failed.");
    } else {
        throw ExpectationFailed(message);
    }
}

}

void fail(const String& message) {
    if (message.empty()) {
        throw ExpectationFailed("kktest::fail: Test failed.");
    } else {
        throw ExpectationFailed("kktest::fail: " + message);
    }
}

}
