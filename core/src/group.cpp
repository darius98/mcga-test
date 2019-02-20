#include "core/src/group.hpp"

#include "core/src/errors.hpp"

using namespace std;

namespace kktest {

Group::Group(GroupConfig&& config, Ptr parentGroup, int index):
        GroupConfig(move(config)),
        parentGroup(move(parentGroup)),
        index(index) {}

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
