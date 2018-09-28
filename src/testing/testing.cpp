#include <fstream>
#include <utility>

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

int numFailedTests() {
    return getDriver()->getNumFailedTests();
}

int writeTestSuiteReport(ostream &report) {
    return getDriver()->generateTestReport(report);
}

int finalizeTesting(const string& reportFileName) {
    ofstream reportFileStream(reportFileName);
    writeTestSuiteReport(reportFileStream);
    reportFileStream.close();
    int status = numFailedTests();
    destroyTestingDriver();
    return status;
}

namespace __internal {

TestDefiner::TestDefiner(string _file, int _line):
        file(move(_file)), line(_line) {}

void TestDefiner::operator()(string description,
                             const function<void()>& func) {
    getDriver()->addTest(new Test(move(description), file, line), func);
}

GroupDefiner::GroupDefiner(string _file, int _line):
        file(move(_file)), line(_line) {}

void GroupDefiner::operator()(string description,
                              const function<void()>& func) {
    getDriver()->addGroup(new Group(move(description), file, line), func);
}

} // namespace __internal

} // namespace runtime_testing

