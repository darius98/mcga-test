#include <fstream>

#include <EasyFlags.hpp>

#include "driver.hpp"
#include "testing.hpp"

using namespace easyflags;
using namespace kktest;
using namespace std;

AddArgument(string, reportFileName)
    .Name("report")
    .Short("r")
    .ArgumentType("FILE ")
    .Description("Generate a JSON test report at the end of running the tests")
    .DefaultValue("")
    .ImplicitValue("./report.json");

int main(int argc, char** argv) {
    ParseEasyFlags(argc, argv);
    TestingDriver::init(argv[0]);

    testCase();

    if (!reportFileName.empty()) {
        ofstream reportFileStream(reportFileName);
        TestingDriver::generateTestReport(reportFileStream);
        reportFileStream.close();
    }
    return TestingDriver::destroy();
}
