#include "core/src/group.hpp"

#include "core/src/errors.hpp"

using namespace std;

namespace kktest {

Group::Group(GroupConfig&& _config, Ptr _parentGroup, int _index):
        GroupConfig(move(_config)),
        parentGroup(move(_parentGroup)),
        index(_index) {}

string Group::getDescription() const {
    return description;
}

void Group::addSetUp(Executable func) {
    if (setUpFunc) {
        throw ConfigurationError(
            "Trying to add second setUp to group \"" + description + "\".");
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
            "Trying to add second tearDown to group \"" + description + "\".");
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

GroupPtr Group::getParentGroup() const {
    return parentGroup;
}

}
