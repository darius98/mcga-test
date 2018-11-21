#include <chrono>

#include "executor.hpp"
#include "errors.hpp"

using namespace std;
using namespace std::chrono;

namespace kktest {

Executor::Executor(): timeTickLengthMs(computeTimeTickLengthFromHardware()) {}

Executor::~Executor() = default;

double Executor::computeTimeTickLengthFromHardware() {
     // TODO: Don't hardcode this, estimate it based on how much a series of computations takes.
    return 1000.0;
}

bool Executor::isDuringTest() const {
    return state == ACTIVE;
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
    auto begin = high_resolution_clock::now();
    Group* group = test->getGroup();
    bool failed = runSetUpsRecursively(group, &failureMessage);
    failed = runTest(func, failed ? nullptr : &failureMessage) || failed;
    failed = runTearDownsRecursively(group, failed ? nullptr : &failureMessage) || failed;
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

bool Executor::runSetUpsRecursively(Group* group, string* failureMessage) {
    if (group == nullptr) {
        return false;
    }

    bool failed = runSetUpsRecursively(group->getParentGroup(), failureMessage);

    try {
        group->setUp();
        return false;
    } catch(const ExpectationFailed& failure) {
        if (!failed && failureMessage != nullptr) {
            failureMessage->assign(failure.what());
        }
        return true;
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const exception& e) {
        if (!failed && failureMessage != nullptr) {
            failureMessage->assign(group->getRenderedFailureMessageOnExceptionInSetUp(e.what()));
        }
    } catch(...) {
        if (!failed && failureMessage != nullptr) {
            failureMessage->assign(group->getRenderedFailureMessageOnNonExceptionInSetUp());
        }
    }
    return true;
}

 bool Executor::runTest(Executable func, string* failureMessage) {
    try {
        func();
        return false;
    } catch(const ExpectationFailed& failure) {
        if (failureMessage != nullptr) {
            failureMessage->assign(failure.what());
        }
        return true;
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const exception& e) {
        if (failureMessage != nullptr) {
            failureMessage->assign("An exception was thrown during test: " + string(e.what()));
        }
    } catch(...) {
        if (failureMessage != nullptr) {
            failureMessage->assign("A non-exception object was thrown during test");
        }
    }
    return true;
}

bool Executor::runTearDownsRecursively(Group* group, string* failureMessage) {
    if (group == nullptr) {
        return false;
    }

    bool failed = true;
    try {
        group->tearDown();
        failed = false;
    } catch(const ExpectationFailed& failure) {
        if (failureMessage != nullptr) {
            failureMessage->assign(failure.what());
        }
        return true;
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const exception& e) {
        if (failureMessage != nullptr) {
            failureMessage->assign(group->getRenderedFailureMessageOnExceptionInTearDown(e.what()));
        }
    } catch(...) {
        if (failureMessage != nullptr) {
            failureMessage->assign(group->getRenderedFailureMessageOnNonExceptionInTearDown());
        }
    }

    return runTearDownsRecursively(group->getParentGroup(), failed ? nullptr : failureMessage);
}

}
