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

void Group::addSetUp(Executable func, const string& file, int line) {
    if (hasSetUp) {
        throw ConfigurationError(
                "Trying to add second kkSetUp to group '" + getFullDescription() + "'."
        );
    }
    hasSetUp = true;
    setUpFunc = func;
    setUpFile = file;
    setUpLine = line;
}

void Group::setUp() const {
    if (hasSetUp) {
        setUpFunc();
    }
}

void Group::addTearDown(Executable func, const string& file, int line) {
    if (hasTearDown) {
        throw ConfigurationError(
                "Trying to add second kkTearDown to group '" + getFullDescription() + "'."
        );
    }
    hasTearDown = true;
    tearDownFunc = func;
    tearDownFile = file;
    tearDownLine = line;
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

string Group::getRenderedFailureMessageOnExceptionInSetUp(const string& what) const {
    return "Exception thrown in kkSetUp"
           " (" + setUpFile + ":" + to_string(setUpLine) + "): " + what;
}

string Group::getRenderedFailureMessageOnNonExceptionInSetUp() const {
    return "Non-exception thrown in kkSetUp"
           " (" + setUpFile + ":" + to_string(setUpLine) + ").";
}

string Group::getRenderedFailureMessageOnExceptionInTearDown(const string& what) const {
    return "Exception thrown in kkTearDown"
           " (" + tearDownFile + ":" + to_string(tearDownLine) + "): " + what;
}

string Group::getRenderedFailureMessageOnNonExceptionInTearDown() const {
    return "Non-exception thrown in kkTearDown"
           " (" + tearDownFile + ":" + to_string(tearDownLine) + ").";
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
