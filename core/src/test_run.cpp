#include "core/src/test_run.hpp"

#include "core/src/errors.hpp"
#include "core/src/group.hpp"

using namespace std;

namespace kktest {

void ExecutionInfo::fail(const string& _failure) {
    if (passed) {
        passed = false;
        failure = _failure;
    }
}

TestRun::TestRun(Test&& test, string failure):
    test(move(test)), info{-1.0, false, move(failure)} {}

TestRun::TestRun(Test&& test, ExecutionInfo&& executionInfo):
        test(move(test)), info(move(executionInfo)) {}

Group* TestRun::getGroup() const {
    return test.getGroup();
}

int TestRun::getGroupIndex() const {
    return test.getGroup()->getIndex();
}

string TestRun::getTestDescription() const {
    return test.getDescription();
}

int TestRun::getTestIndex() const {
    return test.getIndex();
}

bool TestRun::isTestOptional() const {
    return test.isOptional();
}

bool TestRun::isPassed() const {
    return info.passed;
}

string TestRun::getFailure() const {
    return info.failure;
}

}
