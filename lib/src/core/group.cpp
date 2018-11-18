#include "errors.hpp"
#include "group.hpp"

using namespace std;


namespace kktest {

Group::Group(const GroupConfig& _config, Group* _parentGroup, int _index):
        config(_config), parentGroup(_parentGroup), index(_index) {}

const GroupConfig& Group::getConfig() const {
    return config;
}

bool Group::isTopLevel() const {
    return parentGroup != nullptr && parentGroup->parentGroup == nullptr;
}

void Group::addSetUp(Executable func) {
    if (hasSetUp) {
        throw ConfigurationError("Group '" + getFullDescription() + "' already has a setUp!");
    }
    hasSetUp = true;
    setUpFunc = func;
}

void Group::setUp() const {
    if (hasSetUp) {
        setUpFunc();
    }
}

void Group::addTearDown(Executable func) {
    if (hasTearDown) {
        throw ConfigurationError("Group '" + getFullDescription() + "' already has a tearDown!");
    }
    hasTearDown = true;
    tearDownFunc = func;
}

void Group::tearDown() const {
    if (hasTearDown) {
        tearDownFunc();
    }
}

string Group::getFullDescription() const {
    string fullDescription = config.description;
    if (!isTopLevel()) {
        fullDescription = parentGroup->getFullDescription() + "::" + fullDescription;
    }
    return fullDescription;
}

int Group::getIndex() const {
    return index;
}

Group* Group::getParentGroup() const {
    return parentGroup;
}

int Group::getParentGroupIndex() const {
    return parentGroup->getIndex();
}

void Group::markTestStartedExecution() {
    ++ testsStarted;
    if (parentGroup != nullptr) {
        parentGroup->markTestStartedExecution();
    }
}

void Group::markTestFinishedExecution() {
    ++ testsFinished;
    Group* parent = parentGroup;
    if (testsFinished == testsStarted && allTestsStarted) {
        afterAllTestsCallback();
    }
    if (parent != nullptr) {
        parent->markTestFinishedExecution();
    }
}

void Group::markAllTestsStartedExecution(Executable _afterAllTestsCallback) {
    allTestsStarted = true;
    if (testsFinished == testsStarted) {
        _afterAllTestsCallback();
    } else {
        afterAllTestsCallback = _afterAllTestsCallback;
    }
}

}
