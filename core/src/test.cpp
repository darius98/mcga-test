#include "core/src/test.hpp"

#include "core/src/group.hpp"

using namespace std;

namespace kktest {

Test::Test(TestConfig&& config, GroupPtr group, int index):
        TestConfig(move(config)), group(move(group)), index(index) {}

string Test::getDescription() const {
    return description;
}

bool Test::isOptional() const {
    return optional;
}

double Test::getTimeTicksLimit() const {
    return timeTicksLimit;
}

GroupPtr Test::getGroup() const {
    return group;
}

int Test::getIndex() const {
    return index;
}

ExecutedTest::Info::Info(): timeTicks(-1.0), passed(true), failure() {}

ExecutedTest::Info::Info(string&& failure):
        timeTicks(-1.0), passed(false), failure(move(failure)) {}

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

void ExecutedTest::Info::fail(const string& failure) {
    if (passed) {
        passed = false;
        this->failure = failure;
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
