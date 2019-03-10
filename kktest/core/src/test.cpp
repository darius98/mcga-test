#include "kktest/core/src/test.hpp"

#include <algorithm>

using namespace std;

namespace kktest {

Test::Test(TestConfig config, Executable body, GroupPtr group, int id):
        TestConfig(move(config)),
        body(move(body)),
        group(move(group)),
        id(id) {}

const string& Test::getDescription() const {
    return description;
}

bool Test::isOptional() const {
    return optional || group->isOptional();
}

double Test::getTimeTicksLimit() const {
    return timeTicksLimit;
}

std::size_t Test::getNumAttempts() const {
    return attempts;
}

std::size_t Test::getNumRequiredPassedAttempts() const {
    return requiredPassedAttempts;
}

GroupPtr Test::getGroup() const {
    return group;
}

vector<GroupPtr> Test::getGroupStack() const {
    vector<GroupPtr> groupStack(1, getGroup());
    while (groupStack.back()->getParentGroup() != nullptr) {
        groupStack.push_back(groupStack.back()->getParentGroup());
    }
    reverse(groupStack.begin(), groupStack.end());
    return groupStack;
}

int Test::getId() const {
    return id;
}

void Test::run() const {
    body();
}

}
