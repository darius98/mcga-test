#include "core/src/test_run.hpp"

#include "core/src/errors.hpp"
#include "core/src/group.hpp"

using namespace kktest::interproc;
using namespace std;

namespace kktest {

TestRun::TestRun(Test* _test, string _failure):
        test(_test), timeTicks(-1.0), passed(false), failure(move(_failure)) {}

TestRun::TestRun(Test* _test, Message& message): test(_test) {
    MessageStatus status;
    message >> status;
    if (status == CONFIGURATION_ERROR) {
        string errorMessage;
        message >> errorMessage;
        throw ConfigurationError(errorMessage);
    }
    message >> timeTicks >> passed >> failure;
}

TestRun::TestRun(Test* _test, double _timeTicks, bool _passed, string _failure):
        test(_test),
        timeTicks(_timeTicks),
        passed(_passed),
        failure(move(_failure)) {}

Test* TestRun::getTest() const {
    return test;
}

bool TestRun::isPassed() const {
    return passed;
}

TestInfo TestRun::toTestInfo() const {
    return TestInfo(test->getGroup()->getIndex(),
                    test->getIndex(),
                    test->getConfig().optional,
                    test->getConfig().description,
                    passed,
                    failure);
}

Message TestRun::toMessage() const {
    return Message::build(FINISHED_SUCCESSFULLY, timeTicks, passed, failure);
}

}
