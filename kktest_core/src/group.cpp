#include "errors.hpp"
#include "group.hpp"

using std::string;

namespace kktest {

Group::Group(const GroupConfig& _config, Group* _parentGroup, int _index):
        config(_config), parentGroup(_parentGroup), index(_index) {}

const GroupConfig& Group::getConfig() const {
    return config;
}

void Group::addSetUp(Executable func) {
    if (setUpFunc) {
        throw ConfigurationError(
            "Trying to add second setUp to group \"" + config.description + "\"."
        );
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
                "Trying to add second tearDown to group \"" + config.description + "\"."
        );
    }
    tearDownFunc = func;
}

void Group::tearDown() const {
    if (tearDownFunc) {
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
    return "Exception thrown in setUp of group \"" + config.description + "\": " + what;
}

string Group::getRenderedFailureMessageOnNonExceptionInSetUp() const {
    return "Non-exception thrown in setUp of group \"" + config.description + "\".";
}

string Group::getRenderedFailureMessageOnExceptionInTearDown(const string& what) const {
    return "Exception thrown in tearDown of group \"" + config.description + "\": " + what;
}

string Group::getRenderedFailureMessageOnNonExceptionInTearDown() const {
    return "Non-exception thrown in tearDown of group \"" + config.description + "\".";
}

GroupInfo Group::getGroupInfo() const {
    return GroupInfo{
        parentGroup->index,
        index,
        config.description
    };
}

}
