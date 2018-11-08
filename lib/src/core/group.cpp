#include <cstring>

#include "group.hpp"

using namespace std;


namespace kktest {

int Group::globalGroupIndex = 0;

Group::Group(string _description, string _file, int _line, Group* _parentGroup):
        description(move(_description)),
        file(move(_file)),
        line(_line),
        parentGroup(_parentGroup),
        index(++globalGroupIndex) {}

Group::~Group() {
    for (Test* test: tests) {
        delete test;
    }
}

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

Test* Group::addTest(string description, string file, int line, int index) {
    Test* test = new Test(move(description), move(file), line, this, index);
    tests.push_back(test);
    return test;
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

}
