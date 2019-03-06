#include "kktest/core/src/group.hpp"

#include "kktest/core/src/errors.hpp"

using namespace std;

namespace kktest {

Group::Group(GroupConfig config, Ptr parentGroup, int index):
        GroupConfig(move(config)),
        parentGroup(move(parentGroup)),
        index(index) {}

string Group::getDescription() const {
    return description;
}

bool Group::isOptional() const {
    return optional || (parentGroup != nullptr && parentGroup->isOptional());
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
    setUpFunc = move(func);
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
    tearDownFunc = move(func);
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
