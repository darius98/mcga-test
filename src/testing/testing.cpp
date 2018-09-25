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

int getTestReport(ostream& report) {
    return getDriver()->generateTestReport(report);
}

int numFailedTests() {
    return getDriver()->getNumFailedTests();
}

}
