#include "core/src/executor.hpp"

#include "common/utils/src/process_time.hpp"
#include "core/src/errors.hpp"

using namespace kktest::utils;
using namespace std;

namespace kktest {

Executor::Executor(OnTestFinished _onTestFinished):
        onTestFinished(move(_onTestFinished)) {}

Executor::~Executor() = default;

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

void Executor::execute(Test&& test, Executable func) {
    ExecutedTest::Info info = run(test.getGroup(), func);
    onTestFinished(ExecutedTest(move(test), move(info)));
}

ExecutedTest::Info Executor::run(GroupPtr group, Executable func) {
    state = ACTIVE;
    ExecutedTest::Info info;
    info.passed = true;
    ProcessTimer t;
    runSetUps(group, &info);
    runTest(func, &info);
    runTearDowns(group, &info);
    double elapsedMs = 1.0 * t.elapsed().totalNs() / Duration::kMilliToNano;
    info.timeTicks = elapsedMs / getTimeTickLengthMs();
    state = INACTIVE;
    return info;
}

void Executor::runSetUps(GroupPtr group, ExecutedTest::Info* executionInfo) {
    if (group == nullptr) {
        return;
    }

    runSetUps(group->getParentGroup(), executionInfo);
    try {
        group->setUp();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        executionInfo->fail(failure.what());
    } catch(const exception& e) {
        executionInfo->fail("Exception thrown in setUp of group \""
                            + group->getDescription() + "\": " + e.what());
    } catch(...) {
        executionInfo->fail("Non-exception thrown in setUp of group \""
                            + group->getDescription() + "\".");
    }
}

void Executor::runTest(Executable func, ExecutedTest::Info* executionInfo) {
    try {
        func();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        executionInfo->fail(failure.what());
    } catch(const exception& e) {
        executionInfo->fail("An exception was thrown during test: "
                            + string(e.what()));
    } catch(...) {
        executionInfo->fail("A non-exception object was thrown during test");
    }
}

void Executor::runTearDowns(GroupPtr group, ExecutedTest::Info* executionInfo) {
    if (group == nullptr) {
        return;
    }

    try {
        group->tearDown();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& failure) {
        executionInfo->fail(failure.what());
    } catch(const exception& e) {
        executionInfo->fail("Exception thrown in tearDown of group \"" +
                            group->getDescription() + "\": " + e.what());
    } catch(...) {
        executionInfo->fail("Non-exception thrown in tearDown of group \"" +
                            group->getDescription() + "\".");
    }
    runTearDowns(group->getParentGroup(), executionInfo);
}

}
