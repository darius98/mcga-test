#include <cstring>

#include "group.hpp"

using namespace std;


namespace kktest {

Group::Group(string _description,
             string _file,
             int _line,
             Group* _parentGroup,
             int _index,
             Executable _afterAllTestsCallback):
        description(move(_description)),
        file(move(_file)),
        line(_line),
        parentGroup(_parentGroup),
        index(_index),
        afterAllTestsCallback(_afterAllTestsCallback) {}

Group::~Group() = default;

string Group::getDescription() const {
    return description;
}

string Group::getFilename() const {
    return file;
}

int Group::getLine() const {
    return line;
}

bool Group::isGlobalScope() const {
    return parentGroup == nullptr;
}

void Group::addSetUp(Executable func) {
    if (hasSetUp) {
        throw runtime_error("Group '" + getFullDescription() +
                            "' already has a setUp!");
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
        throw runtime_error("Group '" + getFullDescription() +
                            "' already has a tearDown!");
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
    string fullDescription = description;
    if (!parentGroup->isGlobalScope()) {
        fullDescription = parentGroup->getFullDescription()
                          + "::"
                          + fullDescription;
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
        << line
        << file.size()
        << file
        << description.size()
        << description;
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

void Group::markAllTestsStartedExecution() {
    allTestsStarted = true;
}

}
