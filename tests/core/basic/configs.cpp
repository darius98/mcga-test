#include <iostream>
#include <type_traits>

#include "../../../src/core/driver.hpp"
#include "../../../src/core/scan_executor.hpp"
#include "mcga/test.hpp"

using namespace mcga::test;

class TestingDriver : public Driver {
  public:
    std::unique_ptr<TestConfig> testConfig;
    std::unique_ptr<GroupConfig> groupConfig;

    using Driver::Driver;

    void addGroup(GroupConfig config, const Executable& body) override {
        groupConfig = std::make_unique<GroupConfig>(std::move(config));
    }

    void addTest(TestConfig config, Executable body) override {
        testConfig = std::make_unique<TestConfig>(std::move(config));
    }
};

int exitCode = 0;
TestingDriver* driver;

void expectTestConfig(const std::string& description,
                      bool optional,
                      double timeTicksLimit,
                      size_t attempts,
                      size_t requiredPassedAttempts) {
    if (driver->testConfig->description != description) {
        std::cout << "Expected test description to be " << description
                  << ", got " << driver->testConfig->description << "\n";
        exitCode = 1;
    }
    if (driver->testConfig->optional != optional) {
        std::cout << std::boolalpha << "Expected test optional flag to be "
                  << optional << ", got " << driver->testConfig->optional
                  << "\n";
        exitCode = 1;
    }
    if (driver->testConfig->timeTicksLimit != timeTicksLimit) {
        std::cout << "Expected test time ticks limit to be " << timeTicksLimit
                  << ", got " << driver->testConfig->timeTicksLimit << "\n";
        exitCode = 1;
    }
    if (driver->testConfig->attempts != attempts) {
        std::cout << "Expected test time ticks limit to be " << attempts
                  << ", got " << driver->testConfig->attempts << "\n";
        exitCode = 1;
    }
    if (driver->testConfig->requiredPassedAttempts != requiredPassedAttempts) {
        std::cout << "Expected test time ticks limit to be "
                  << requiredPassedAttempts << ", got "
                  << driver->testConfig->attempts << "\n";
        exitCode = 1;
    }
}

void expectGroupConfig(const std::string& description, bool optional) {
    if (driver->groupConfig->description != description) {
        std::cout << "Expected group description to be " << description
                  << ", got " << driver->groupConfig->description << "\n";
        exitCode = 1;
    }
    if (driver->groupConfig->optional != optional) {
        std::cout << std::boolalpha << "Expected group optional flag to be "
                  << optional << ", got " << driver->groupConfig->optional
                  << "\n";
        exitCode = 1;
    }
}

void executable() {
}

int main() {
    ExtensionApi api;
    ScanExecutor executor(&api);
    TestingDriver testingDriver(&executor);

    driver = &testingDriver;
    Driver::Init(&testingDriver);

    // Common cases of test configs.

    // Default test
    test("desc", executable, Context());
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
    group("desc", executable, Context());
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
