#include "group.hpp"

using namespace std;

namespace mcga::test {

Group::Group(GroupConfig config, Ptr parentGroup, int id)
        : GroupConfig(move(config)), parentGroup(move(parentGroup)), id(id) {
}

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
    setUpFunc = move(func);
}

bool Group::hasSetUp() const {
    return setUpFunc != nullptr;
}

void Group::setUp() const {
    if (hasSetUp()) {
        setUpFunc();
    }
}

void Group::addTearDown(Executable func) {
    tearDownFunc = move(func);
}

bool Group::hasTearDown() const {
    return tearDownFunc != nullptr;
}

void Group::tearDown() const {
    if (hasTearDown()) {
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

}  // namespace mcga::test
