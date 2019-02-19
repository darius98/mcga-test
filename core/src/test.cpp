#include "core/src/test.hpp"

#include "core/src/group.hpp"

using namespace std;

namespace kktest {

Test::Test(TestConfig&& _config, Group* _group, int _index):
        TestConfig(move(_config)), group(_group), index(_index) {}

std::string Test::getDescription() const {
    return description;
}

bool Test::isOptional() const {
    return optional;
}

double Test::getTimeTicksLimit() const {
    return timeTicksLimit;
}

Group* Test::getGroup() const {
    return group;
}

int Test::getIndex() const {
    return index;
}

ExecutedTest::Info::Info(): timeTicks(-1.0), passed(true), failure() {}

ExecutedTest::Info::Info(string&& _failure):
        timeTicks(-1.0), passed(false), failure(move(_failure)) {}

ExecutedTest::Info::Info(Info&& other) noexcept:
        timeTicks(other.timeTicks),
        passed(other.passed),
        failure(move(other.failure)) {}

ExecutedTest::Info& ExecutedTest::Info::operator=(Info&& other) noexcept {
    if (this != &other) {
        timeTicks = other.timeTicks;
        passed = other.passed;
        failure = move(other.failure);
    }
    return *this;
}

void ExecutedTest::Info::fail(const string& _failure) {
    if (passed) {
        passed = false;
        failure = _failure;
    }
}

ExecutedTest::ExecutedTest(Test&& test, string&& failure):
        Test(move(test)), info(move(failure)) {}

ExecutedTest::ExecutedTest(Test&& test, Info&& info):
        Test(move(test)), info(move(info)) {}

bool ExecutedTest::isPassed() const {
    return info.passed;
}

string ExecutedTest::getFailure() const {
    return info.failure;
}

}
