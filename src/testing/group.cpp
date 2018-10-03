#include "group.hpp"

using namespace std;

namespace ktest {

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

int Group::generateTestReport(ostream& report, size_t spaces) {
    string prefix(spaces + 2, ' ');
    report << "{\n";
    if (!description.empty()) {
        report << prefix << R"("description": ")" << description << "\",\n";
    }
    if (!file.empty()) {
        report << prefix << R"("file": ")" << file << "\",\n";
        report << prefix << "\"line\": " << line << ",\n";
    }
    report << prefix << "\"numTests\": " << numTests << ",\n";
    report << prefix << "\"numFailedTests\": " << numFailedTests;
    if (!tests.empty() || !subGroups.empty()) {
        report << ",";
    }
    report << "\n";
    if (!tests.empty()) {
        report << prefix << "\"tests\": [";
        for (size_t i = 0; i < tests.size(); ++ i) {
            if (i == 0) {
                report << "\n" << prefix;
            }
            report << "  ";
            tests[i]->generateTestReport(report, spaces + 4);
            if (i + 1 != tests.size()) {
                report << ",";
            }
            report << "\n" << prefix;
        }
        report << "]";
    }
    if (!subGroups.empty()) {
        report << ",";
    }
    report << "\n";
    if (!subGroups.empty()) {
        report << prefix << "\"subGroups\": [";
        for (size_t i = 0; i < subGroups.size(); ++i) {
            if (i == 0) {
                report << "\n" << prefix;
            }
            report << "  ";
            subGroups[i]->generateTestReport(report, spaces + 4);
            if (i + 1 != subGroups.size()) {
                report << ",";
            }
            report << "\n" << prefix;
        }
        report << "]\n";
    }
    report << string(spaces, ' ') << "}";
    return numFailedTests;
}

}
