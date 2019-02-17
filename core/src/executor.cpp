#include "core/src/executor.hpp"

#include "common/utils/src/time.hpp"
#include "core/src/errors.hpp"

using namespace kktest::utils;
using namespace std;

namespace kktest {

Executor::Executor(OnTestFinishedCallback _onTestFinishedCallback):
        onTestFinishedCallback(move(_onTestFinishedCallback)),
        timeTickLengthMs(computeTimeTickLengthFromHardware()) {}

Executor::~Executor() = default;

double Executor::computeTimeTickLengthFromHardware() {
    // TODO(darius98): Don't hard-code this, estimate it based on how much a
    // series of computations takes.
    return 1000.0;
}

void Executor::checkIsInactive(const string& methodName) const {
    if (state == ACTIVE) {
        throw ConfigurationError(methodName + " called in invalid context.");
    }
}

double Executor::getTimeTickLengthMs() const {
    return timeTickLengthMs;
}

void Executor::finalize() {}

void Executor::execute(Test* test, Executable func) {
    auto executionInfo = run(test, func);
    test->setExecuted(executionInfo);
    onTestFinishedCallback(test);
}

TestExecutionInfo Executor::run(Test* test, Executable func) {
    state = ACTIVE;
    string failureMessage;
    bool failed = false;
    auto setFailure = [&failureMessage, &failed](const string& value) {
        if (!failed) {
            failed = true;
            failureMessage = value;
        }
    };
    ProcessTimer t;
    Group* group = test->getGroup();
    runSetUpsRecursively(group, setFailure);
    runTest(func, setFailure);
    runTearDownsRecursively(group, setFailure);
    double executionTimeMs = t.getMsElapsed();
    state = INACTIVE;
    TestExecutionInfo result;
    result.executionTimeTicks = executionTimeMs / timeTickLengthMs;
    result.passed = !failed;
    result.failureMessage = failureMessage;
    return result;
}

void Executor::runSetUpsRecursively(Group* group, SetFailure setFailure) {
    if (group == nullptr) {
        return;
    }

    runSetUpsRecursively(group->getParentGroup(), setFailure);
    try {
        group->setUp();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        setFailure(failure.what());
    } catch(const exception& e) {
        setFailure("Exception thrown in setUp of group \""
                   + group->getDescription() + "\": " + e.what());
    } catch(...) {
        setFailure("Non-exception thrown in setUp of group \""
                   + group->getDescription() + "\".");
    }
}

void Executor::runTest(Executable func, SetFailure setFailure) {
    try {
        func();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        setFailure(failure.what());
    } catch(const exception& e) {
        setFailure("An exception was thrown during test: " + string(e.what()));
    } catch(...) {
        setFailure("A non-exception object was thrown during test");
    }
}

void Executor::runTearDownsRecursively(Group* group, SetFailure setFailure) {
    if (group == nullptr) {
        return;
    }

    try {
        group->tearDown();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        setFailure(failure.what());
    } catch(const exception& e) {
        setFailure("Exception thrown in tearDown of group \"" +
                   group->getDescription() + "\": " + e.what());
    } catch(...) {
        setFailure("Non-exception thrown in tearDown of group \"" +
                   group->getDescription() + "\".");
    }
    runTearDownsRecursively(group->getParentGroup(), setFailure);
}

}
