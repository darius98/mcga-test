#include "testing.hpp"

#include "expect.hpp"
#include "driver.hpp"

using namespace std;


namespace runtime_testing {

static TestingDriver* testingDriver = nullptr;

TestingDriver* getDriver() {
    if (testingDriver == nullptr) {
        testingDriver = new TestingDriver();
    }
    return testingDriver;
}

void initializeTestingDriver(std::ostream& log) {
    if (testingDriver != nullptr) {
        throw runtime_error("Testing driver cannot be initialized: "
                            "it already exists.");
    }
    testingDriver = new TestingDriver(&log);
}

void destroyTestingDriver() {
    delete testingDriver;
    testingDriver = nullptr;
}

bool isDuringTest() {
    return testingDriver != nullptr && testingDriver->isDuringTest();
}

void setUp(const function<void()>& func) {
    return getDriver()->addSetUp(func);
}

void tearDown(const function<void()>& func) {
    return getDriver()->addTearDown(func);
}

void group(const string& description, const function<void()>& func) {
    return getDriver()->addGroup(new Group(description), func);
}

void test(const string& description,
          const function<void()>& func) {
    return getDriver()->addTest(new Test(description), func);
}

int numFailedTests() {
    return getDriver()->getNumFailedTests();
}

int getTestSuiteReport(ostream& report) {
    return getDriver()->generateTestReport(report);
}

}
