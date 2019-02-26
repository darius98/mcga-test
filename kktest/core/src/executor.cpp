#include "kktest/core/src/executor.hpp"

#include "common/utils/src/time.hpp"
#include "kktest/core/src/errors.hpp"

using namespace kktest::utils;
using namespace std;

namespace kktest {

Executor::Executor(OnTestFinished onTestFinished):
        onTestFinished(move(onTestFinished)) {}

double Executor::getTimeTickLengthMs() {
    static double timeTickLengthMs = -1.0;
    if (timeTickLengthMs == -1.0) {
        // TODO(darius98): Don't hard-code this, estimate it based on how much a
        //  series of computations takes.
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

void Executor::execute(Test&& test, const Executable& func) {
    vector<ExecutedTest::Info> executions;
    executions.reserve(static_cast<size_t>(test.getNumAttempts()));
    for (int i = 0; i < test.getNumAttempts(); ++ i) {
        executions.push_back(run(test.getGroup(), func));
    }
    onTestFinished(ExecutedTest(move(test), move(executions)));
}

ExecutedTest::Info Executor::run(GroupPtr group, const Executable& func) {
    state = ACTIVE;
    ExecutedTest::Info info;
    RealTimeTimer t;
    runSetUps(group, &info);
    runTest(func, &info);
    runTearDowns(group, &info);
    double elapsedMs = 1.0 * t.elapsed().totalNs() / Duration::kMilliToNano;
    info.timeTicks = elapsedMs / getTimeTickLengthMs();
    state = INACTIVE;
    return info;
}

void Executor::runSetUps(GroupPtr group, ExecutedTest::Info* execution) {
    if (group == nullptr) {
        return;
    }

    runSetUps(group->getParentGroup(), execution);
    try {
        group->setUp();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        execution->fail(failure.what());
    } catch(const exception& e) {
        execution->fail("Exception thrown in setUp of group \""
                        + group->getDescription() + "\": " + e.what());
    } catch(...) {
        execution->fail("Non-exception thrown in setUp of group \""
                        + group->getDescription() + "\".");
    }
}

void Executor::runTest(const Executable& func, ExecutedTest::Info* execution) {
    try {
        func();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        execution->fail(failure.what());
    } catch(const exception& e) {
        execution->fail("An exception was thrown during test: "
                        + string(e.what()));
    } catch(...) {
        execution->fail("A non-exception object was thrown during test");
    }
}

void Executor::runTearDowns(GroupPtr group, ExecutedTest::Info* execution) {
    if (group == nullptr) {
        return;
    }

    try {
        group->tearDown();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        execution->fail(failure.what());
    } catch(const exception& e) {
        execution->fail("Exception thrown in tearDown of group \""
                        + group->getDescription() + "\": " + e.what());
    } catch(...) {
        execution->fail("Non-exception thrown in tearDown of group \""
                        + group->getDescription() + "\".");
    }
    runTearDowns(group->getParentGroup(), execution);
}

}
