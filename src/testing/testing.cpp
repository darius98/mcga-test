#include <fstream>

#include <EasyFlags.hpp>

#include "testing/driver.hpp"

using namespace easyflags;
using namespace kktest;
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

void initializeTesting(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    TestingDriver::initGlobal(argv[0]);
}

int finalizeTesting() {
    if (flagEnableReport) {
        ofstream reportFileStream(reportFileName);
        TestingDriver::generateTestReport(reportFileStream);
        reportFileStream.close();
    }
    return TestingDriver::destroyGlobal();
}
