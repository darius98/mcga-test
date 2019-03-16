#include "kktest/core/group.hpp"

#include "kktest/core/warning.hpp"

using namespace std;

namespace kktest {

Group::Group(GroupConfig config, Ptr parentGroup, int id):
        GroupConfig(move(config)),
        parentGroup(move(parentGroup)),
        id(id) {}

string Group::getDescription() const {
    return description;
}

bool Group::isOptional() const {
    return optional || (parentGroup != nullptr && parentGroup->isOptional());
}

int Group::getId() const {
    return id;
}

GroupPtr Group::getParentGroup() const {
    return parentGroup;
}

void Group::addSetUp(Executable func) {
    if (setUpFunc) {
        EmitWarning("kktest::setUp() called, but a setUp for group \"",
                    description,
                    "\" already exists. Ignoring...");
        return;
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
        EmitWarning("kktest::tearDown() called, but a tearDown for group \"",
                    description,
                    "\" already exists. Ignoring...");
        return;
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
