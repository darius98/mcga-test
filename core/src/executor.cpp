#include "core/src/executor.hpp"

#include "common/utils/src/process_time.hpp"
#include "core/src/errors.hpp"

using namespace kktest::utils;
using namespace std;

namespace kktest {

Executor::Executor(OnTestFinished _onTestFinishedCallback):
        onTestFinishedCallback(move(_onTestFinishedCallback)) {}

Executor::~Executor() = default;

double Executor::getTimeTickLengthMs() {
    static double timeTickLengthMs = -1.0;
    if (timeTickLengthMs == -1.0) {
        // TODO(darius98): Don't hard-code this, estimate it based on how much a
        // series of computations takes.
        timeTickLengthMs = 1000.0;
    }
    return timeTickLengthMs;
}

void Executor::checkIsInactive(const string& methodName) const {
    if (state == ACTIVE) {
        throw ConfigurationError(methodName + " called in invalid context.");
    }
}

void Executor::finalize() {}

void Executor::execute(Test* test, Executable func) {
    auto executionInfo = run(test, func);
    onTestFinishedCallback(test, executionInfo);
}

ExecutionInfo Executor::run(Test* test, Executable func) {
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
    double executionMs = 1.0 * t.elapsed().totalNs() / Duration::kMilliToNano;
    state = INACTIVE;
    return ExecutionInfo(
        /*timeTicks=*/executionMs / getTimeTickLengthMs(),
        /*passed=*/!failed,
        /*failure=*/failureMessage);
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
