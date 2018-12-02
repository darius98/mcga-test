#include "errors.hpp"
#include "group.hpp"

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

String Group::getRenderedFailureMessageOnExceptionInSetUp(const String& what) const {
    return "Exception thrown in setUp of group \"" + config.description + "\": " + what;
}

String Group::getRenderedFailureMessageOnNonExceptionInSetUp() const {
    return "Non-exception thrown in setUp of group \"" + config.description + "\".";
}

String Group::getRenderedFailureMessageOnExceptionInTearDown(const String& what) const {
    return "Exception thrown in tearDown of group \"" + config.description + "\": " + what;
}

String Group::getRenderedFailureMessageOnNonExceptionInTearDown() const {
    return "Non-exception thrown in tearDown of group \"" + config.description + "\".";
}

GroupInfo Group::getGroupInfo() const {
    int parentGroupIndex = -1;
    if (parentGroup != nullptr) {
        parentGroupIndex = parentGroup->index;
    }
    return GroupInfo{
        parentGroupIndex,
        index,
        config.description
    };
}

}
