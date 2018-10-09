#include "group.hpp"

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

void Group::setSetUp(Executable func) {
    if (hasSetUp) {
        throw runtime_error("Group '" + description +
                            "' already has a setUp!");
    }
    hasSetUp = true;
    setUpFunc = func;
}

void Group::setTearDown(Executable func) {
    if (hasTearDown) {
        throw runtime_error("Group '" + description +
                            "' already has a tearDown!");
    }
    hasTearDown = true;
    tearDownFunc = func;
}

string Group::getFullDescription() const {
    string fullDescription = description;
    if (parentGroup->parentGroup != nullptr) {
        fullDescription = parentGroup->getFullDescription() + "::" + fullDescription;
    }
    return fullDescription;
}

JSON Group::generateReport() const {
    JSON report = {
        {"file", file},
        {"line", line},
        {"numTests", numTests},
        {"numFailedTests", numFailedTests},
    };
    if (!description.empty()) {
        report["description"] = description;
    }
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
