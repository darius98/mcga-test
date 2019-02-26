#include "kktest/core/src/test.hpp"

#include "kktest/core/src/group.hpp"

using namespace std;

namespace kktest {

Test::Test(TestConfig&& config, GroupPtr group, int index):
        TestConfig(move(config)), group(move(group)), index(index) {}

const string& Test::getDescription() const {
    return description;
}

bool Test::isOptional() const {
    return optional || group->isOptional();
}

double Test::getTimeTicksLimit() const {
    return timeTicksLimit;
}

int Test::getNumAttempts() const {
    return attempts;
}

int Test::getNumRequiredPassedAttempts() const {
    return requiredPassedAttempts;
}

GroupPtr Test::getGroup() const {
    return group;
}

int Test::getIndex() const {
    return index;
}

}
