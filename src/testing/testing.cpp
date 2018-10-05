#include <fstream>

#include <EasyFlags.hpp>

#include "testing/driver.hpp"

using namespace easyflags;
using namespace std;

AddArgument(int, flagEnableReport)
    .Name("enable-report")
    .ArgumentType("bool")
    .Description("Enable test report generation for this test run")
    .DefaultValue(1).ImplicitValue(1);
AddArgument(string, reportFileName)
    .Name("report-file")
    .ArgumentType("string")
    .Description("Path to file where to dump the test report")
    .DefaultValue("report.json");


namespace kktest {

void initializeTestingDriver(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    TestingDriver::initGlobal();
}

int finalizeTesting() {
    TestingDriver* driver = TestingDriver::getGlobalDriver();
    if (flagEnableReport) {
        ofstream reportFileStream(reportFileName);
        driver->generateTestReport(reportFileStream);
        reportFileStream.close();
    }
    int status = driver->getNumFailedTests();
    TestingDriver::destroyGlobal();
    return status;
}

} // namespace kktest
