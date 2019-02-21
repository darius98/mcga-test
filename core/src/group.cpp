#include "core/src/group.hpp"

#include "core/src/errors.hpp"

using namespace std;

namespace kktest {

Group::Group(GroupConfig&& config, Ptr parentGroup, int index):
        GroupConfig(move(config)),
        parentGroup(move(parentGroup)),
        index(index) {}

string Group::getDescription() const {
    return description;
}

int Group::getIndex() const {
    return index;
}

GroupPtr Group::getParentGroup() const {
    return parentGroup;
}

void Group::addSetUp(Executable func) {
    if (setUpFunc) {
        throw ConfigurationError(
            "Trying to add second setUp to group \"" + description + "\".");
    }
    setUpFunc = func;
}

void Group::setUp() const {
    if (setUpFunc) {
        setUpFunc();
    }
}

void Group::addTearDown(Executable func) {
    if (tearDownFunc) {
        throw ConfigurationError(
            "Trying to add second tearDown to group \"" + description + "\".");
    }
    tearDownFunc = func;
}

void Group::tearDown() const {
    if (tearDownFunc) {
        tearDownFunc();
    }
}

void Group::addStartedTest() {
    testsStarted += 1;
    if (parentGroup != nullptr) {
        parentGroup->addStartedTest();
    }
}

void Group::setStartedAllTests() {
    allTestsStarted = true;
}

void Group::addFinishedTest() {
    testsFinished += 1;
    if (parentGroup != nullptr) {
        parentGroup->addFinishedTest();
    }
}

bool Group::finishedAllTests() const {
    return allTestsStarted && testsStarted == testsFinished;
}

}
