#include <iostream>
#include <type_traits>

#include "core/driver.hpp"
#include "core/scan_executor.hpp"
#include "mcga/test.hpp"

using namespace mcga::test;

std::unique_ptr<TestConfig> testConfig;
std::unique_ptr<GroupConfig> groupConfig;

namespace mcga::test::internal {

void register_test(TestConfig config, Executable exe) {
    testConfig = std::make_unique<TestConfig>(move(config));
}

void register_group(GroupConfig config, Executable exe) {
    groupConfig = std::make_unique<GroupConfig>(move(config));
}

}

int exitCode = 0;

void expectTestConfig(const String& description,
                      bool optional,
                      double timeTicksLimit,
                      size_t attempts,
                      size_t requiredPassedAttempts) {
    if (testConfig->description.c_str() != description.c_str()) {
        std::cout << "Expected test description to be " << description.c_str()
                  << ", got " << testConfig->description.c_str()
                  << "\n";
        exitCode = 1;
    }
    if (testConfig->optional != optional) {
        std::cout << std::boolalpha << "Expected test optional flag to be "
                  << optional << ", got " << testConfig->optional
                  << "\n";
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
                  << requiredPassedAttempts << ", got "
                  << testConfig->attempts << "\n";
        exitCode = 1;
    }
}

void expectGroupConfig(const String& description, bool optional) {
    if (groupConfig->description.c_str() != description.c_str()) {
        std::cout << "Expected group description to be " << description.c_str()
                  << ", got " << groupConfig->description.c_str()
                  << "\n";
        exitCode = 1;
    }
    if (groupConfig->optional != optional) {
        std::cout << std::boolalpha << "Expected group optional flag to be "
                  << optional << ", got " << groupConfig->optional
                  << "\n";
        exitCode = 1;
    }
}

void executable() {
}

int main() {
    // Common cases of test configs.

    // Default test
    test("desc", executable, Context());
    expectTestConfig("desc", false, 1.0, 1, 1);

    // Anonymous test
    test(executable);
    expectTestConfig("", false, 1.0, 1, 1);

    // Optional test
    test.optional("desc", executable);
    expectTestConfig("desc", true, 1.0, 1, 1);

    // Anonymous optional test
    test.optional(executable);
    expectTestConfig("", true, 1.0, 1, 1);

    // Multi-run test
    test.multiRun(100, "desc", executable);
    expectTestConfig("desc", false, 1.0, 100, 100);

    // Anonymous multi-run test
    test.multiRun(100, executable);
    expectTestConfig("", false, 1.0, 100, 100);

    // Optional multi-run test
    test.optional.multiRun(100, "desc", executable);
    expectTestConfig("desc", true, 1.0, 100, 100);

    // Anonymous optional multi-run test
    test.optional.multiRun(100, executable);
    expectTestConfig("", true, 1.0, 100, 100);

    // Retry test
    test.retry(100, "desc", executable);
    expectTestConfig("desc", false, 1.0, 100, 1);

    // Anonymous retry test
    test.retry(100, executable);
    expectTestConfig("", false, 1.0, 100, 1);

    // Optional retry test
    test.optional.retry(100, "desc", executable);
    expectTestConfig("desc", true, 1.0, 100, 1);

    // Anonymous optional retry test
    test.optional.retry(100, executable);
    expectTestConfig("", true, 1.0, 100, 1);

    // Common cases of group configs.

    // Default group
    group("desc", executable, Context());
    expectGroupConfig("desc", false);

    // Anonymous group
    group(executable);
    expectGroupConfig("", false);

    // Optional group
    group.optional("desc", executable);
    expectGroupConfig("desc", true);

    // Optional anonymous group
    group.optional(executable);
    expectGroupConfig("", true);

    return exitCode;
}
