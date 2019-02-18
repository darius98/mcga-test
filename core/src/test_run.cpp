#include "core/src/test_run.hpp"

#include "core/src/errors.hpp"
#include "core/src/group.hpp"

using namespace kktest::interproc;
using namespace std;

namespace kktest {

TestRun::TestRun(Test&& test, string failure):
    test(move(test)), timeTicks(-1.0), passed(false), failure(move(failure)) {}

TestRun::TestRun(Test&& test, Message& message): test(move(test)) {
    MessageStatus status;
    message >> status;
    if (status == CONFIGURATION_ERROR) {
        string errorMessage;
        message >> errorMessage;
        throw ConfigurationError(errorMessage);
    }
    message >> timeTicks >> passed >> failure;
}

TestRun::TestRun(Test&& test, double timeTicks, bool passed, string failure):
        test(move(test)),
        timeTicks(timeTicks),
        passed(passed),
        failure(move(failure)) {}

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
    return passed;
}

string TestRun::getFailure() const {
    return failure;
}

Message TestRun::toMessage() const {
    return Message::build(FINISHED_SUCCESSFULLY, timeTicks, passed, failure);
}

}
