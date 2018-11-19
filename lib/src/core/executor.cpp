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
    return state == TEST;
}

void Executor::checkIsInactive(const string& methodName, const string& file, int line) const {
    string errorPrefix = file + ":" + to_string(line) + ": " + methodName;
    if (state == TEST) {
        throw ConfigurationError(errorPrefix + " called within kkTest.");
    }
    if (state == SET_UP) {
        throw ConfigurationError(errorPrefix + " called within kkSetUp.");
    }
    if (state == TEAR_DOWN) {
        throw ConfigurationError(errorPrefix + " called within kkTearDown.");
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
    state = SET_UP;
    string failureMessage;
    auto begin = high_resolution_clock::now();
    bool failed = runSetUpsRecursively(test->getGroup(), &failureMessage);
    if (!failed) {
        state = TEST;
        failed = runTest(func, &failureMessage);
        if (!failed) {
            failed = runTearDownsRecursively(test->getGroup(), &failureMessage);
        }
    }
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
    if (failed) {
        return true;
    }

    try {
        group->setUp();
        return false;
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const exception& e) {
        failureMessage->assign(group->getRenderedFailureMessageOnExceptionInSetUp(e.what()));
    } catch(...) {
        failureMessage->assign(group->getRenderedFailureMessageOnNonExceptionInSetUp());
    }
    return true;
}

 bool Executor::runTest(Executable func, string* failureMessage) {
    try {
        func();
        return false;
    } catch(const ExpectationFailed& failure) {
        failureMessage->assign(failure.what());
        return true;
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const exception& e) {
        failureMessage->assign("An exception was thrown during test: " + string(e.what()));
    } catch(...) {
        failureMessage->assign("A non-exception object was thrown during test");
    }
    return true;
}

bool Executor::runTearDownsRecursively(Group* group, string* failureMessage) {
    if (group == nullptr) {
        return false;
    }

    try {
        group->tearDown();
        return false;
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const exception& e) {
        failureMessage->assign(group->getRenderedFailureMessageOnExceptionInTearDown(e.what()));
    } catch(...) {
        failureMessage->assign(group->getRenderedFailureMessageOnNonExceptionInTearDown());
    }
    return true;
}

}
