#include "group.hpp"

using namespace std;

namespace runtime_testing {

void Group::generateTestReport(ostream& report,
                               const string& currentGroupFullName) {
    for (Test* test: this->tests) {
        test->report(report, currentGroupFullName);
        delete test->failure;
        delete test;
    }
    for (Group* group: this->subGroups) {
        group->generateTestReport(
                report,
                currentGroupFullName + " > " + group->description
        );
        delete group;
    }
}

int Group::getNumFailedTests() {
    int num = 0;
    for (Test* test: this->tests) {
        num += (test->failure != nullptr);
    }
    for (Group* group: this->subGroups) {
        num += group->getNumFailedTests();
    }
    return num;
}

}
