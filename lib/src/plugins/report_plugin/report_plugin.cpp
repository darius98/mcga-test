#include <fstream>

#include <EasyFlags.hpp>

#include <core/driver.hpp>
#include "report_plugin.hpp"

using namespace autojson;
using namespace std;

AddArgument(string, argumentReportFileName)
    .Name("report")
    .Short("r")
    .ArgumentType("FILE ")
    .Description("Generate a JSON test report at the end of running the tests")
    .DefaultValue("")
    .ImplicitValue("./report.json");

namespace kktest {

bool ReportPlugin::isEnabled() const {
    return !argumentReportFileName.empty();
}

void ReportPlugin::install() {
    report["numTests"] = 0;
    report["numFailedTests"] = 0;

    TestingDriver::addBeforeGroupHook([this](Group* group) {
        partialGroupJSONs[group] = map<string, JSON>{
            {"line", group->getLine()},
            {"file", group->getFilename()},
            {"description", group->getDescription()},
            {"index", group->getIndex()}
        };
    });

    TestingDriver::addAfterGroupHook([this](Group* group) {
        JSON groupJSON = partialGroupJSONs[group];
        JSON& parentJSON = group->getParentGroup()->isGlobalScope() ? report:
                                partialGroupJSONs[group->getParentGroup()];
        if (!parentJSON.exists("subGroups")) {
            parentJSON["subGroups"] = vector<JSON>();
        }
        parentJSON["subGroups"].push_back(groupJSON);
    });

    TestingDriver::addAfterTestHook([this](Test* test) {
        JSON testJSON = {
            {"description", test->getConfig().description},
            {"optional", test->getConfig().optional},
            {"file", test->getFilename()},
            {"line", test->getLine()},
            {"executed", test->isExecuted()},
            {"index", test->getIndex()}
        };
        if (test->isExecuted()) {
            report["numTests"] = (int)report["numTests"] + 1;
            testJSON["passed"] = test->isPassed();
            if (!test->isPassed()) {
                report["numFailedTests"] = (int)report["numFailedTests"] + 1;
                testJSON["failureMessage"] = test->getFailureMessage();
            }
        }

        JSON& parentJSON = test->getParentGroup()->isGlobalScope() ? report:
                                partialGroupJSONs[test->getParentGroup()];
        if (!parentJSON.exists("tests")) {
            parentJSON["tests"] = vector<JSON>();
        }
        parentJSON["tests"].push_back(testJSON);
    });

    TestingDriver::addBeforeDestroyHook([this]() {
        ofstream reportFile(argumentReportFileName);
        reportFile << report.stringify(0);
        reportFile.close();
    });
}

}
