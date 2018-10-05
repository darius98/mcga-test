#include <fstream>

#include <EasyFlags.hpp>

#include "testing/driver.hpp"

using namespace easyflags;
using namespace std;


namespace kktest {

void initializeTestingDriver(int argc, char** argv) {
    TestingDriver::initGlobal();
    ParseEasyFlags(argc, argv);
}

void destroyTestingDriver() {
    TestingDriver::destroyGlobal();
}

bool isDuringTest() {
    return TestingDriver::isDuringTest();
}

void setUp(const function<void()>& func) {
    return TestingDriver::getGlobalDriver()->addSetUp(func);
}

void tearDown(const function<void()>& func) {
    return TestingDriver::getGlobalDriver()->addTearDown(func);
}

int numFailedTests() {
    return TestingDriver::getGlobalDriver()->getNumFailedTests();
}

void writeTestSuiteReport(ostream &report) {
    TestingDriver::getGlobalDriver()->generateTestReport(report);
}

int finalizeTesting(const string& reportFileName) {
    ofstream reportFileStream(reportFileName);
    writeTestSuiteReport(reportFileStream);
    reportFileStream.close();
    int status = numFailedTests();
    destroyTestingDriver();
    return status;
}

} // namespace kktest
