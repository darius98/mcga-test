#include <iostream>
#include <type_traits>

#include "mcga/test.hpp"

using namespace mcga::test;

namespace mcga::test {

std::unique_ptr<TestConfig> testConfig;
void test(TestConfig config, Executable) {
    testConfig = std::make_unique<TestConfig>(std::move(config));
}

std::unique_ptr<GroupConfig> groupConfig;
void group(GroupConfig config, const Executable&) {
    groupConfig = std::make_unique<GroupConfig>(std::move(config));
}

}  // namespace mcga::test

int exitCode = 0;

void expectTestConfig(const std::string& description,
                      bool optional,
                      double timeTicksLimit,
                      size_t attempts,
                      size_t requiredPassedAttempts) {
    if (testConfig->description != description) {
        std::cout << "Expected test description to be " << description
                  << ", got " << testConfig->description << "\n";
        exitCode = 1;
    }
    if (testConfig->optional != optional) {
        std::cout << std::boolalpha << "Expected test optional flag to be "
                  << optional << ", got " << testConfig->optional << "\n";
        exitCode = 1;
    }
    if (testConfig->timeTicksLimit != timeTicksLimit) {
        std::cout << "Expected test time ticks limit to be " << timeTicksLimit
                  << ", got " << testConfig->timeTicksLimit << "\n";
        exitCode = 1;
    }
    if (testConfig->attempts != attempts) {
        std::cout << "Expected test time ticks limit to be " << attempts
                  << ", got " << testConfig->attempts << "\n";
        exitCode = 1;
    }
    if (testConfig->requiredPassedAttempts != requiredPassedAttempts) {
        std::cout << "Expected test time ticks limit to be "
                  << requiredPassedAttempts << ", got " << testConfig->attempts
                  << "\n";
        exitCode = 1;
    }
}

void expectGroupConfig(const std::string& description, bool optional) {
    if (groupConfig->description != description) {
        std::cout << "Expected group description to be " << description
                  << ", got " << groupConfig->description << "\n";
        exitCode = 1;
    }
    if (groupConfig->optional != optional) {
        std::cout << std::boolalpha << "Expected group optional flag to be "
                  << optional << ", got " << groupConfig->optional << "\n";
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