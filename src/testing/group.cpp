#include "testing/group.hpp"

using namespace autojson;
using namespace std;


namespace kktest {

Group::Group(string description, string file, int line):
        description(move(description)), file(move(file)), line(line) {}

Group::~Group() {
    for (Test* t: tests) {
        delete t;
    }
    for (Group* g: subGroups) {
        delete g;
    }
}

JSON Group::generateReport() const {
    JSON report;
    if (!description.empty()) {
        report["description"] = description;
    }
    if (!file.empty()) {
        report["file"] = file;
        report["line"] = line;
    }
    report["numTests"] = numTests;
    report["numFailedTests"] = numFailedTests;
    if (!tests.empty()) {
        report["tests"] = vector<JSON>();
        for (const auto& test: tests) {
            report["tests"].push_back(test->generateReport());
        }
    }
    if (!subGroups.empty()) {
        report["subGroups"] = vector<JSON>();
        for (const auto& subGroup: subGroups) {
            report["subGroups"].push_back(subGroup->generateReport());
        }
    }
    return report;
}

}
