#include <chrono>

#include <core/errors.hpp>
#include <core/executor.hpp>

using namespace std;
using namespace std::chrono;

namespace kktest {

Executor::Executor(): timeTickLengthMs(computeTimeTickLengthFromHardware()) {}

Executor::~Executor() = default;

double Executor::computeTimeTickLengthFromHardware() {
     // TODO: Don't hardcode this, estimate it based on how much a series of computations takes.
    return 1000.0;
}

void Executor::checkIsInactive(const string& methodName, const string& file, int line) const {
    if (state == ACTIVE) {
        throw ConfigurationError(
            file + ":" + to_string(line) + ": " + methodName + " called in invalid context."
        );
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
    string failureMessage;
    bool failed = false;
    auto setFailure = [&failureMessage, &failed](const string& value) {
        if (!failed) {
            failed = true;
            failureMessage = value;
        }
    };
    auto begin = high_resolution_clock::now();
    Group* group = test->getGroup();
    runSetUpsRecursively(group, setFailure);
    runTest(func, setFailure);
    runTearDownsRecursively(group, setFailure);
    auto end = high_resolution_clock::now();
    state = INACTIVE;
    setTestExecuted(test,
        /* executionTimeTicks=*/duration_cast<milliseconds>(end - begin).count() / timeTickLengthMs,
        /*             passed=*/!failed,
        /*     failureMessage=*/failureMessage
    );
}

void Executor::setTestExecuted(Test* test,
                               double executionTimeTicks,
                               bool passed,
                               const string& failureMessage) {
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
        setFailure("An exception was thrown during test: " + string(e.what()));
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

}
