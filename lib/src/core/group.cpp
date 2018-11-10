#include "group.hpp"

using namespace std;


namespace kktest {

Group::Group(const GroupConfig& _config, Group* _parentGroup, int _index):
        config(_config), parentGroup(_parentGroup), index(_index) {}

const GroupConfig& Group::getConfig() const {
    return config;
}

bool Group::isGlobalScope() const {
    return parentGroup == nullptr;
}

void Group::addSetUp(Executable func) {
    if (hasSetUp) {
        throw runtime_error("Group '" + getFullDescription() + "' already has a setUp!");
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
        throw runtime_error("Group '" + getFullDescription() + "' already has a tearDown!");
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
    if (!parentGroup->isGlobalScope()) {
        fullDescription = parentGroup->getFullDescription() + "::" + fullDescription;
    }
    return fullDescription;
}

int Group::getIndex() const {
    return index;
}

void Group::writeBytes(BytesConsumer& consumer) const {
    if (isGlobalScope()) {
        return;
    }
    consumer
        << (parentGroup->isGlobalScope() ? 0 : parentGroup->getIndex())
        << index
        << config.line
        << config.file.size()
        << config.file
        << config.description.size()
        << config.description;
}

Group* Group::getParentGroup() const {
    return parentGroup;
}

void Group::markTestStartedExecution() {
    ++ testsStarted;
}

void Group::markTestFinishedExecution() {
    ++ testsFinished;
    if (testsFinished == testsStarted && allTestsStarted) {
        afterAllTestsCallback();
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
