#include "kktest/core/executor.hpp"

#include "common/utils/time.hpp"
#include "kktest/core/errors.hpp"
#include "kktest/core/time_tick.hpp"

using namespace kktest::utils;
using namespace std;

namespace kktest {

void Executor::setOnTestFinishedCallback(OnTestFinished _onTestFinished) {
    onTestFinished = move(_onTestFinished);
}

void Executor::setOnWarningCallback(OnWarning _onWarning) {
    onWarning = move(_onWarning);
}

bool Executor::isActive() const {
    return state != INACTIVE;
}

string Executor::stateAsString() const {
    switch (state) {
        case INACTIVE: return "inactive";
        case INSIDE_TEST: return "test";
        case INSIDE_SET_UP: return "setUp";
        case INSIDE_TEAR_DOWN: return "tearDown";
    }
}

void Executor::finalize() {}

void Executor::execute(Test test) {
    vector<ExecutedTest::Info> executions;
    executions.reserve(test.getNumAttempts());
    for (size_t i = 0; i < test.getNumAttempts(); ++ i) {
        executions.push_back(run(test));
    }
    onTestFinished(ExecutedTest(move(test), move(executions)));
}

ExecutedTest::Info Executor::run(const Test& test) {
    currentTestGroupId = test.getGroup()->getId();
    currentTestId = test.getId();
    state = INSIDE_SET_UP;
    ExecutedTest::Info info;
    Timer t;
    vector<GroupPtr> testGroupStack = test.getGroupStack();
    vector<GroupPtr>::iterator it;
    // Execute setUp()-s, in the order of the group stack.
    for (it = testGroupStack.begin(); it != testGroupStack.end(); ++ it) {
        runJob([it] { (*it)->setUp(); },
               &info,
               "setUp of group \"" + (*it)->getDescription() + "\"");
        if (!info.passed) {
            // If a setUp() fails, do not execute the rest.
            break;
        }
    }
    state = INSIDE_TEST;
    if (info.passed) {
        // Only run the test if all setUp()-s passed without exception.
        runJob(bind(&Test::run, &test), &info, "test");
        -- it;
    }
    state = INSIDE_TEAR_DOWN;
    // Execute tearDown()-s in reverse order, from where setUp()-s stopped.
    for (; it + 1 != testGroupStack.begin(); -- it) {
        runJob([it] { (*it)->tearDown(); },
               &info,
               "tearDown of group \"" + (*it)->getDescription() + "\"");
    }
    info.timeTicks = 1.0 * t.elapsed().count() / getTimeTickLength().count();
    state = INACTIVE;
    return info;
}

void Executor::handleWarning(const string& message) {
    onWarning(Warning(message, currentTestGroupId, currentTestId));
}

void Executor::runJob(const Executable& job,
                      ExecutedTest::Info* execution,
                      const string& where) {
    try {
        job();
    } catch(const ExpectationFailed& failure) {
        execution->fail(failure.what());
    } catch(const exception& e) {
        execution->fail("Uncaught exception in " + where + ": " + e.what());
    } catch(...) {
        execution->fail("Uncaught non-exception type in " + where + "\".");
    }
}

}
