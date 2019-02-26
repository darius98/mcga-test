#include "kktest/core/src/test.hpp"

#include "kktest/core/src/group.hpp"

using namespace std;

namespace kktest {

Test::Test(TestConfig&& config, Executable body, GroupPtr group, int index):
        TestConfig(move(config)),
        body(move(body)),
        group(move(group)),
        index(index) {}

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

int Test::getIndex() const {
    return index;
}

}
