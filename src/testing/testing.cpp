#include "testing.hpp"

#include "expect.hpp"
#include "driver.hpp"

using namespace std;


namespace runtime_testing {


TestingDriver* getDriver() {
    static TestingDriver* driver = nullptr;
    if (driver == nullptr) {
        driver = new TestingDriver(cout);
    }
    return driver;
}

bool isDuringTest() {
    return getDriver()->isDuringTest();
}

void setUp(const function<void()>& setUpFunc) {
    return getDriver()->addSetUp(setUpFunc);
}

void tearDown(const function<void()>& tearDownFunc) {
    return getDriver()->addTearDown(tearDownFunc);
}

void group(const string& description, const function<void()>& groupFunc) {
    return getDriver()->addGroup(new Group(description), groupFunc);
}

void test(const string& description,
          const function<void()>& testFunc) {
    return getDriver()->addTest(new Test(description), testFunc);
}

int getTestReport(ostream& report) {
    getDriver()->generateTestReport(report);
    return getDriver()->getNumFailedTests();
}

int numFailedTests() {
    return getDriver()->getNumFailedTests();
}

}
