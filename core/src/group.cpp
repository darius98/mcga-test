#include "errors.hpp"
#include "group.hpp"

namespace kktest {

Group::Group(const GroupConfig& _config, Group* _parentGroup, int _index):
        config(_config), parentGroup(_parentGroup), index(_index) {}

String Group::getDescription() const {
    return config.description;
}

void Group::addSetUp(Executable func) {
    if (setUpFunc) {
        throw ConfigurationError(
            "Trying to add second setUp to group "
            "\"" + config.description + "\".");
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
            "Trying to add second tearDown to group "
            "\"" + config.description + "\".");
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

GroupInfo Group::getGroupInfo() const {
    int parentGroupIndex = parentGroup == nullptr ? -1 : parentGroup->index;
    return GroupInfo{parentGroupIndex, index, config.description};
}

}
