#include "group.hpp"

namespace mcga::test {

Group::Group(GroupConfig config, Ptr parentGroup, int id)
        : GroupConfig(std::move(config)), parentGroup(std::move(parentGroup)),
          id(id) {
}

std::string Group::getDescription() const {
    return description;
}

bool Group::isOptional() const {
    return optional || (parentGroup != nullptr && parentGroup->isOptional());
}

int Group::getId() const {
    return id;
}

GroupPtr Group::getParentGroup() const {
    return parentGroup;
}

void Group::addSetUp(Executable func) {
    setUpFuncs.push_back(std::move(func));
}

void Group::setUp() const {
    for (const auto& setUpFunc: setUpFuncs) {
        setUpFunc();
    }
}

void Group::addTearDown(Executable func) {
    tearDownFuncs.push_back(move(func));
}

void Group::tearDown() const {
    for (auto it = tearDownFuncs.rbegin(); it != tearDownFuncs.rend(); it++) {
        (*it)();
    }
}

}  // namespace mcga::test
