#include "errors.hpp"
#include "group.hpp"

using namespace std;

namespace kktest {

Group::Group(const GroupConfig& _config, Group* _parentGroup, int _index):
        config(_config), parentGroup(_parentGroup), index(_index) {}

string Group::getFileAndLine() const {
    return config.file + "::" + to_string(config.line);
}

void Group::addSetUp(Executable func, const string& file, int line) {
    if (hasSetUp) {
        throw ConfigurationError(
            "Trying to add second kkSetUp to group at " + getFileAndLine() + "."
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
                "Trying to add second kkTearDown to group " + getFileAndLine() + "."
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

GroupInfo Group::getGroupInfo() const {
    return GroupInfo{
        parentGroup->index,
        index,
        config.line,
        config.file,
        config.description
    };
}

}
