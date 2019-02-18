#include "core/src/test.hpp"

#include "core/src/group.hpp"

using namespace kktest::interproc;
using namespace std;

namespace kktest {

Test::Test(const TestConfig& _config, Group* _parentGroup, int _index):
        config(_config), parentGroup(_parentGroup), index(_index) {}

const TestConfig& Test::getConfig() const {
    return config;
}

Group* Test::getGroup() const {
    return parentGroup;
}

int Test::getIndex() const {
    return index;
}

TestInfo Test::toTestInfo() const {
    return TestInfo(parentGroup->getIndex(),
                    index,
                    config.optional,
                    config.description,
                    false,
                    "");
}

}
