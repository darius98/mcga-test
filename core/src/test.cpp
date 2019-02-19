#include "core/src/test.hpp"

#include "core/src/group.hpp"

using namespace std;

namespace kktest {

Test::Test(TestConfig&& _config, Group* _group, int _index):
        TestConfig(move(_config)), group(_group), index(_index) {}

Test::Test(Test&& other) noexcept = default;
Test& Test::operator=(Test&& other) noexcept = default;

Group* Test::getGroup() const {
    return group;
}

int Test::getIndex() const {
    return index;
}

void ExecutedTest::Info::fail(const string& _failure) {
    if (passed) {
        passed = false;
        failure = _failure;
    }
}

ExecutedTest::ExecutedTest(Test&& test, string failure):
    Test(move(test)), info{-1.0, false, move(failure)} {}

ExecutedTest::ExecutedTest(Test&& test, Info&& info):
        Test(move(test)), info(move(info)) {}

bool ExecutedTest::isPassed() const {
    return info.passed;
}

string ExecutedTest::getFailure() const {
    return info.failure;
}

}
