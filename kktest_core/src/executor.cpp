#include <kktest_common/time.hpp>
#include "errors.hpp"
#include "executor.hpp"

using kktest::utils::Timer;
using std::exception;
using std::function;

namespace kktest {

Executor::Executor(): timeTickLengthMs(computeTimeTickLengthFromHardware()) {}

Executor::~Executor() = default;

double Executor::computeTimeTickLengthFromHardware() {
    // TODO(darius98): Don't hard-code this, estimate it based on how much a
    // series of computations takes.
    return 1000.0;
}

void Executor::checkIsInactive(const String& methodName) const {
    if (state == ACTIVE) {
        throw ConfigurationError(methodName + " called in invalid context.");
    }
}

double Executor::getTimeTickLengthMs() const {
    return timeTickLengthMs;
}

void Executor::finalize() {}

void Executor::execute(Test* test, Executable func) {
    run(test, func);
}

void Executor::onTestFinished(const function<void(Test*)>& _onTestFinishedCallback) {
    onTestFinishedCallback = _onTestFinishedCallback;
}

void Executor::run(Test* test, Executable func) {
    state = ACTIVE;
    String failureMessage;
    bool failed = false;
    auto setFailure = [&failureMessage, &failed](const String& value) {
        if (!failed) {
            failed = true;
            failureMessage = value;
        }
    };
    Timer t;
    Group* group = test->getGroup();
    runSetUpsRecursively(group, setFailure);
    runTest(func, setFailure);
    runTearDownsRecursively(group, setFailure);
    double executionTimeMs = t.getMillisecondsElapsed();
    state = INACTIVE;
    setTestExecuted(test, executionTimeMs / timeTickLengthMs, !failed, failureMessage);
}

void Executor::setTestExecuted(Test* test,
                               double executionTimeTicks,
                               bool passed,
                               const String& failureMessage) {
    test->setExecuted(executionTimeTicks, passed, failureMessage);
    if (onTestFinishedCallback) {
        onTestFinishedCallback(test);
    }
}

void Executor::runSetUpsRecursively(Group* group, SetFailureType setFailure) {
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
        setFailure(group->getRenderedFailureMessageOnExceptionInSetUp(e.what()));
    } catch(...) {
        setFailure(group->getRenderedFailureMessageOnNonExceptionInSetUp());
    }
}

void Executor::runTest(Executable func, SetFailureType setFailure) {
    try {
        func();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        setFailure(failure.what());
    } catch(const exception& e) {
        setFailure("An exception was thrown during test: " + String(e.what()));
    } catch(...) {
        setFailure("A non-exception object was thrown during test");
    }
}

void Executor::runTearDownsRecursively(Group* group, SetFailureType setFailure) {
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
        setFailure(group->getRenderedFailureMessageOnExceptionInTearDown(e.what()));
    } catch(...) {
        setFailure(group->getRenderedFailureMessageOnNonExceptionInTearDown());
    }
    runTearDownsRecursively(group->getParentGroup(), setFailure);
}

}  // namespace kktest
