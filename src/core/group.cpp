#include "group.hpp"

namespace mcga::test {

Group::Group(GroupConfig config, Context context, Ptr parentGroup, int id)
        : GroupConfig(std::move(config)), context(std::move(context)),
          parentGroup(std::move(parentGroup)), id(id) {
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

void Group::addTearDown(UserTestExecutable func) {
    tearDownFuncs.push_back(std::move(func));
}

}  // namespace mcga::test
