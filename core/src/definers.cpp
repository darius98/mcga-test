#include <cstring>

#include <iostream>

#include <kktest_impl/definers.hpp>
#include <kktest_impl/signature.hpp>
#include "driver.hpp"
#include "test_case_registry.hpp"

using namespace std;

namespace kktest {

TestCaseDefiner::TestCaseDefiner(void (*testCase)(),
                                 const char* name,
                                 const unsigned char* signature) noexcept {
    if (memcmp(signature, kkTestSignature, kkTestSigSize) != 0) {
        cout << "Invalid signature passed to TestCaseDefiner. If you didn't do "
                "anything weird when defining test cases, then this is a bug, "
                "please report.\n";
        exit(1);
    }
    TestCaseRegistry::add(testCase, name);
}

void test(const string& description, Executable func) {
    test(testConfig(_.description = description), func);
}

void test(const TestConfig& config, Executable func) {
    Driver::getInstance()->addTest(config, func);
}

void group(const string& description, Executable func) {
    group(groupConfig(_.description = description), func);
}

void group(const GroupConfig& config, Executable func) {
    Driver::getInstance()->addGroup(config, func);
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
