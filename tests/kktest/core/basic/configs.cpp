#include <cstdlib>

#include <iostream>
#include <type_traits>

#include "kktest/core/include/kktest.hpp"

using namespace kktest;
using namespace std;

namespace kktest {

TestConfig* testConfig = nullptr;
void test(TestConfig config, Executable) {
    delete testConfig;
    testConfig = new TestConfig(move(config));
}

GroupConfig* groupConfig = nullptr;
void group(GroupConfig config, const Executable&) {
    delete groupConfig;
    groupConfig = new GroupConfig(move(config));
}

}  // namespace kktest

int exitCode = 0;

void expectTestConfig(const string& description,
                      bool optional,
                      double timeTicksLimit,
                      size_t attempts,
                      size_t requiredPassedAttempts) {
    if (testConfig->description != description) {
        cout << "Expected test description to be " << description << ", got "
             << testConfig->description << "\n";
        exitCode = 1;
    }
    if (testConfig->optional != optional) {
        cout << boolalpha << "Expected test optional flag to be " << optional
             << ", got " << testConfig->optional << "\n";
        exitCode = 1;
    }
    if (testConfig->timeTicksLimit != timeTicksLimit) {
        cout << "Expected test time ticks limit to be " << timeTicksLimit
             << ", got " << testConfig->timeTicksLimit << "\n";
        exitCode = 1;
    }
    if (testConfig->attempts != attempts) {
        cout << "Expected test time ticks limit to be " << attempts << ", got "
             << testConfig->attempts << "\n";
        exitCode = 1;
    }
    if (testConfig->requiredPassedAttempts != requiredPassedAttempts) {
        cout << "Expected test time ticks limit to be "
             << requiredPassedAttempts << ", got " << testConfig->attempts
             << "\n";
        exitCode = 1;
    }
}

void expectGroupConfig(const string& description, bool optional) {
    if (groupConfig->description != description) {
        cout << "Expected group description to be " << description << ", got "
             << groupConfig->description << "\n";
        exitCode = 1;
    }
    if (groupConfig->optional != optional) {
        cout << boolalpha << "Expected group optional flag to be " << optional
             << ", got " << groupConfig->optional << "\n";
        exitCode = 1;
    }
}

void executable() {
}

int main() {
    // Common cases of test configs.

    // Default test
    test("desc", executable);
    expectTestConfig("desc", false, 1.0, 1, 1);

    // Anonymous test
    test(executable);
    expectTestConfig("(anonymous test)", false, 1.0, 1, 1);

    // Optional test
    optionalTest("desc", executable);
    expectTestConfig("desc", true, 1.0, 1, 1);

    // Anonymous optional test
    optionalTest(executable);
    expectTestConfig("(anonymous test)", true, 1.0, 1, 1);

    // Multi-run test
    multiRunTest("desc", 100, executable);
    expectTestConfig("desc", false, 1.0, 100, 100);

    // Anonymous multi-run test
    multiRunTest(100, executable);
    expectTestConfig("(anonymous test)", false, 1.0, 100, 100);

    // Optional multi-run test
    optionalMultiRunTest("desc", 100, executable);
    expectTestConfig("desc", true, 1.0, 100, 100);

    // Anonymous optional multi-run test
    optionalMultiRunTest(100, executable);
    expectTestConfig("(anonymous test)", true, 1.0, 100, 100);

    // Retry test
    retryTest("desc", 100, executable);
    expectTestConfig("desc", false, 1.0, 100, 1);

    // Anonymous retry test
    retryTest(100, executable);
    expectTestConfig("(anonymous test)", false, 1.0, 100, 1);

    // Optional retry test
    optionalRetryTest("desc", 100, executable);
    expectTestConfig("desc", true, 1.0, 100, 1);

    // Anonymous optional retry test
    optionalRetryTest(100, executable);
    expectTestConfig("(anonymous test)", true, 1.0, 100, 1);

    // Common cases of group configs.

    // Default group
    group("desc", executable);
    expectGroupConfig("desc", false);

    // Anonymous group
    group(executable);
    expectGroupConfig("(anonymous group)", false);

    // Optional group
    optionalGroup("desc", executable);
    expectGroupConfig("desc", true);

    // Optional anonymous group
    optionalGroup(executable);
    expectGroupConfig("(anonymous group)", true);

    return exitCode;
}