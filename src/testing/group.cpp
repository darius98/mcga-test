#include "group.hpp"

using namespace std;

namespace runtime_testing {

Group::Group(const std::string &description): description(description) {}

int Group::generateTestReport(ostream& report,
                              const string& currentGroupFullName) {
    int numFailedTests = 0;
    for (Test* test: this->tests) {
        test->report(report, currentGroupFullName);
        numFailedTests += (test->failure != nullptr);
    }
    for (Group* group: this->subGroups) {
        numFailedTests += group->generateTestReport(
            report,
            currentGroupFullName + " > " + group->description
        );
    }
}

int Group::getNumFailedTests() {
    int numFailedTests = 0;
    for (Test* test: this->tests) {
        numFailedTests += (test->failure != nullptr);
    }
    for (Group* group: this->subGroups) {
        numFailedTests += group->getNumFailedTests();
    }
    return numFailedTests;
}

}
