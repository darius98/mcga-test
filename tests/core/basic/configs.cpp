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

    void addGroup(GroupConfig config, Executable body) override {
        groupConfig = std::make_unique<GroupConfig>(std::move(config));
    }

    void addTest(TestConfig config, Executable body) override {
        testConfig = std::make_unique<TestConfig>(std::move(config));
    }
};

int exitCode = 0;
TestingDriver* driver;

void expectTestConfig(const String& description,
                      bool optional,
                      double timeTicksLimit,
                      size_t attempts,
                      size_t requiredPassedAttempts) {
    if (driver->testConfig->description.c_str() != description.c_str()) {
        std::cout << "Expected test description to be " << description.c_str()
                  << ", got " << driver->testConfig->description.c_str()
                  << "\n";
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

void expectGroupConfig(const String& description, bool optional) {
    if (driver->groupConfig->description.c_str() != description.c_str()) {
        std::cout << "Expected group description to be " << description.c_str()
                  << ", got " << driver->groupConfig->description.c_str()
                  << "\n";
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
