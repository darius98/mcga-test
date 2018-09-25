#include "testing.hpp"

#include "expect.hpp"
#include "driver.hpp"

using namespace std;


namespace runtime_testing {


TestingDriver* getDriver() {
    static TestingDriver* driver = nullptr;
    if (driver == nullptr) {
        driver = new TestingDriver();
    }
    return driver;
}

bool isDuringTest() {
    return getDriver()->isDuringTest();
}

void _setUp(function<void()> setUpFunc,
            const char* fileName,
            const int& lineNumber) {
    auto driver = getDriver();
    driver->validateStartSetUp(fileName, lineNumber);
    driver->addSetUp(setUpFunc);
}

void _tearDown(function<void()> tearDownFunc,
               const char* fileName,
               const int& lineNumber) {
    auto driver = getDriver();
    driver->validateStartTearDown(fileName, lineNumber);
    driver->addTearDown(tearDownFunc);
}

int _getTestReport(const char* fileName) {
    auto driver = getDriver();
    stringstream report;
    driver->generateTestReport(report, fileName);
    return driver->getNumFailedTests();
}

int numFailedTests() {
    return getDriver()->getNumFailedTests();
}

}

void group(const string& description, const function<void()>& groupFunc) {
    auto driver = runtime_testing::getDriver();
    driver->validateStartGroup();
    auto currentGroup = new runtime_testing::Group(description);
    driver->addGroup(currentGroup, groupFunc);
}

void test(const string& description,
          const function<void()>& testFunc) {
    auto driver = runtime_testing::getDriver();
    driver->validateStartTest();
    auto currentTest = new runtime_testing::Test(description);
    driver->addTest(currentTest, testFunc);
}
