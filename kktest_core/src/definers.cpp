#include <cstring>

#include <iostream>

#include <kktest_impl/definers.hpp>
#include <kktest_impl/signature.hpp>
#include "driver.hpp"
#include "errors.hpp"
#include "test_case_registry.hpp"

using namespace std;

namespace kktest {

TestCaseDefiner::TestCaseDefiner(void (*testCase)(), const unsigned char* signature) {
    if (memcmp(signature, kkTestSignature, kkTestSigSize) != 0) {
        cout << "Invalid signature passed to TestCaseDefiner. If you didn't do anything weird when "
                "defining test cases, then this might be a bug, please report.\n";
        exit(1);
    }
    TestCaseRegistry::add(testCase);
}

Definer::Definer(string _file, int _line): file(move(_file)), line(_line) {}

void TestDefiner::operator()(string description, Executable func) {
    (*this)(kkTestConfig(_.description = description), func);
}

void TestDefiner::operator()(const TestConfig& config, Executable func) {
    TestConfig fullConfig(config);
    fullConfig.file = file;
    fullConfig.line = line;
    TestingDriver::getInstance()->addTest(fullConfig, func);
}

void GroupDefiner::operator()(string description, Executable func) {
    (*this)(kkGroupConfig(_.description = description), func);
}

void GroupDefiner::operator()(const GroupConfig& config, Executable func) {
    GroupConfig fullConfig(config);
    fullConfig.file = file;
    fullConfig.line = line;
    TestingDriver::getInstance()->addGroup(fullConfig, func);
}

void SetUpDefiner::operator()(Executable func) {
    TestingDriver::getInstance()->addSetUp(func, file, line);
}

void TearDownDefiner::operator()(Executable func) {
    TestingDriver::getInstance()->addTearDown(func, file, line);
}

void ExpectDefiner::operator()(const bool& exprResult, const string& expr) {
    if (!exprResult) {
        throwExpectationFailed(file + ":" + to_string(line) + ": " + expr);
    }
}

void ExpectDefiner::throwExpectationFailed(const string& message) {
    throw ExpectationFailed(message);
}

}
