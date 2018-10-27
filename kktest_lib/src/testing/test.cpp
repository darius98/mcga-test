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
        parentGroup(_parentGroup),
        index(++globalTestIndex) {}

Test::~Test() {
    delete failure;
}

int Test::getIndex() const {
    return index;
}

void Test::setExecuted() {
    if (isExecuted()) {
        throw runtime_error("Test::setExecuted called twice on the same test!");
    }
    executed = true;
    Group* parent = parentGroup;
    while (parent != nullptr) {
        parent->updateWithExecutedTest(this);
        parent = parent->getParentGroup();
    }
}

bool Test::isExecuted() const {
    return executed;
}

bool Test::isFailed() const {
    return isExecuted() && failure != nullptr;
}

bool Test::isPassed() const {
    return isExecuted() && !isFailed();
}

void Test::setFailure(const string& message) {
    if (failure == nullptr) {
        failure = new ExpectationFailed(message);
    }
}

void Test::setFailure(const ExpectationFailed& f) {
    if (failure == nullptr) {
        failure = new ExpectationFailed(f);
    }
}

string Test::getFailureMessage() const {
    return failure->getMessage();
}

string Test::getDescriptionPrefix() const {
    string prefix = file + ":" + to_string(line) + "::";
    if (!parentGroup->isGlobalScope()) {
        prefix += parentGroup->getFullDescription() + "::";
    }
    return prefix;
}

string Test::getDescription() const {
    return description;
}

JSON Test::toJSON() const {
    JSON report = {
        {"description", description},
        {"file", file},
        {"line", line},
        {"executed", isExecuted()}
    };
    if (isExecuted()) {
        report["passed"] = isPassed();
        if (!isPassed()) {
            report["failureMessage"] = getFailureMessage();
        }
    }
    return report;
}

Group* Test::getParentGroup() const {
    return parentGroup;
}

}
