#include "group.hpp"

using namespace std;

namespace runtime_testing {

Group::Group(const std::string &description): description(description) {}

Group::~Group() {
    for (Test* test: this->tests) {
        delete test;
    }
    for (Group* group: this->subGroups) {
        delete group;
    }
}

int Group::generateTestReport(ostream& report, size_t spaces) {
    string prefix(spaces + 2, ' ');
    report << "{\n";
    if (!this->description.empty()) {
        report << prefix
               << R"("description": ")"
               << this->description
               << "\",\n";
    }
    report << prefix << "\"numTests\": " << this->numTests << ",\n";
    report << prefix << "\"numFailedTests\": " << this->numFailedTests << ",\n";
    report << prefix << "\"tests\": [";
    for (size_t i = 0; i < this->tests.size(); ++ i) {
        if (i == 0) {
            report << "\n" << prefix;
        }
        report << "  ";
        this->tests[i]->generateTestReport(report, spaces + 4);
        if (i + 1 != this->tests.size()) {
            report << ",";
        }
        report << "\n" << prefix;
    }
    report << "],\n";
    report << prefix << "\"subGroups\": [";
    for (size_t i = 0; i < this->subGroups.size(); ++ i) {
        if (i == 0) {
            report << "\n" << prefix;
        }
        report << "  ";
        this->subGroups[i]->generateTestReport(report, spaces + 4);
        if (i + 1 != this->subGroups.size()) {
            report << ",";
        }
        report << "\n" << prefix;
    }
    report << "]\n";
    report << string(spaces, ' ') << "}";
    return this->numFailedTests;
}

}
