#include "core/src/test.hpp"

#include "core/src/group.hpp"

using namespace std;

namespace kktest {

Test::Test(const TestConfig& _config, Group* _group, int _index):
        config(_config), group(_group), index(_index) {}
Test::Test(const Test& other) = default;
Test::Test(Test&& other) noexcept = default;

Test& Test::operator=(Test&& other) noexcept = default;

string Test::getDescription() const {
    return config.description;
}

bool Test::isOptional() const {
    return config.optional;
}

double Test::getTimeTicksLimit() const {
    return config.timeTicksLimit;
}

Group* Test::getGroup() const {
    return group;
}

int Test::getIndex() const {
    return index;
}

}
