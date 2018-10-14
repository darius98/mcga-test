#include "test.hpp"
#include "group.hpp"

using namespace autojson;
using namespace std;


namespace kktest {

int Test::globalTestIndex = 0;

Test::Test(string _description, string _file, int _line, Group* _parentGroup):
        description(move(_description)),
        file(move(_file)),
        line(_line),
        parentGroup(_parentGroup) {
    index = ++globalTestIndex;
}

Test::~Test() {
    delete failure;
}

int Test::getIndex() const {
    return index;
}

bool Test::isFailed() const {
    return failure != nullptr;
}

bool Test::isPassed() const {
    return !isFailed();
}

void Test::setFailure(const string& message) {
    if (isPassed()) {
        failure = new ExpectationFailed(message);
    }
}

void Test::setFailure(const ExpectationFailed& f) {
    if (isPassed()) {
        failure = new ExpectationFailed(f);
    }
}

string Test::getFailureMessage() const {
    return failure->getMessage();
}

string Test::getFullDescription() const {
    string fullDescription = file + ":" + to_string(line) + ": ";

    if (!parentGroup->isGlobalScope()) {
        fullDescription += parentGroup->getFullDescription() + "::";
    }

    return fullDescription + description;
}

JSON Test::generateReport() const {
    JSON report = {
        {"description", description},
        {"file", file},
        {"line", line},
        {"passed", isPassed()}
    };
    if (!isPassed()) {
        report["failureMessage"] = getFailureMessage();
    }
    return report;
}

void Test::updateGroups() const {
    Group* parent = parentGroup;
    while (parent != nullptr) {
        parent->numTests += 1;
        parent->numFailedTests += isFailed();
        parent = parent->getParentGroup();
    }
}

}
