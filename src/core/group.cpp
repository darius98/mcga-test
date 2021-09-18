#include "group.hpp"

namespace mcga::test {

Group::Group(GroupConfig config, Ptr parentGroup, int id)
        : GroupConfig(std::move(config)), parentGroup(std::move(parentGroup)),
          id(id) {
}

const std::string& Group::getDescription() const {
    return description;
}

const Context& Group::getContext() const {
    return context;
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

void Group::addSetUp(UserTestExecutable func) {
    setUpFuncs.push_back(std::move(func));
}

void Group::setUp() const {
    for (const auto& setUpFunc: setUpFuncs) {
        setUpFunc.func();
    }
}

void Group::addTearDown(UserTestExecutable func) {
    tearDownFuncs.push_back(std::move(func));
}

void Group::tearDown() const {
    for (auto it = tearDownFuncs.rbegin(); it != tearDownFuncs.rend(); it++) {
        it->func();
    }
}

}  // namespace mcga::test
