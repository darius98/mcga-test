#include "executor.hpp"

#include <chrono>
#include <stdexcept>
#include <thread>

#include "time_tick.hpp"

namespace mcga::test {

class ExpectationFailed : public std::runtime_error {
  public:
    Context context;

    ExpectationFailed(const std::string& what, Context context)
            : std::runtime_error(what), context(std::move(context)) {
    }
};

Executor::Executor(ExtensionApi* api): api(api) {
}

bool Executor::isActive() const {
    return state != INACTIVE;
}

std::string Executor::stateAsString() const {
    switch (state) {
        case INSIDE_TEST: return "test";
        case INSIDE_SET_UP: return "setUp";
        case INSIDE_TEAR_DOWN: return "tearDown";
        default: return "inactive";
    }
}

void Executor::finalize() {
    api->runHooks<ExtensionApi::BEFORE_DESTROY>();
}

void Executor::addFailure(const std::string& failure, Context context) {
    // We only kill the thread on failure if we are in the main testing thread
    // and we know we catch this exception.
    if (std::hash<std::thread::id>()(std::this_thread::get_id())
        == currentExecutionThreadId) {
        // TODO: Abort when exceptions are disabled (in non-smooth execution,
        //  inform the test runner process of the failure before aborting).
        throw ExpectationFailed(failure, std::move(context));
    }

    // If the user starts his own threads that entertain failures, it is his
    // responsibility to make sure his threads die on failure (we have no
    // control).
    std::lock_guard guard(currentExecutionFailureMutex);
    if (!currentExecutionIsFailed) {
        currentExecutionIsFailed = true;
        currentExecutionFailureMessage = failure;
        currentExecutionFailureContext = std::move(context);
    }
}

void Executor::execute(Test test) {
    for (size_t i = 0; i < test.getNumAttempts(); ++i) {
        onTestExecutionStart(test);
        test.addExecution(run(test));
        onTestExecutionFinish(test);
    }
}

Executor::Type Executor::getType() const {
    return SMOOTH;
}

Test::ExecutionInfo Executor::run(const Test& test) {
    currentTest = &test;
    state = INSIDE_SET_UP;
    api->runHooks<ExtensionApi::BEFORE_TEST_SETUP>(test);
    Test::ExecutionInfo info;
    auto startTime = std::chrono::high_resolution_clock::now();
    std::vector<GroupPtr> testGroupStack = test.getGroupStack();
    std::vector<GroupPtr>::iterator it;
    // Execute setUp()-s, in the order of the group stack.
    for (it = testGroupStack.begin(); it != testGroupStack.end(); ++it) {
        (*it)->forEachSetUp([&](const Executable& setUp) {
            currentSetUp = &setUp;
            runJob(setUp, &info);
            currentSetUp = nullptr;
            // If a setUp() fails, do not execute the rest.
            return info.passed;
        });
    }
    state = INSIDE_TEST;
    if (info.passed) {
        // Only run the test if all setUp()-s passed without exception.
        runJob(test.getBody(), &info);
        --it;
    }
    state = INSIDE_TEAR_DOWN;
    // Execute tearDown()-s in reverse order, from where setUp()-s stopped.
    for (; it + 1 != testGroupStack.begin(); --it) {
        (*it)->forEachTearDown([&](const Executable& tearDown) {
            currentTearDown = &tearDown;
            runJob(tearDown, &info);
            currentTearDown = nullptr;
            return true;
        });
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    info.timeTicks = NanosecondsToTimeTicks(endTime - startTime);
    if (info.timeTicks > test.getTimeTicksLimit()) {
        info.fail("Execution timed out.");
    }
    api->runHooks<ExtensionApi::AFTER_TEST_TEARDOWN>(test);
    state = INACTIVE;
    currentTest = nullptr;
    return info;
}

void Executor::emitWarning(Warning warning, GroupPtr group) {
    onWarning(std::move(warning), std::move(group));
}

void Executor::runJob(const Executable& job, Test::ExecutionInfo* execution) {
    currentExecutionThreadId
      = std::hash<std::thread::id>()(std::this_thread::get_id());
    currentExecutionIsFailed = false;
    currentExecutionFailureMessage = "";
    currentExecutionFailureContext = std::nullopt;

    try {
        job();
    } catch (const ExpectationFailed& failure) {
        execution->fail(failure.what(), failure.context);
    } catch (const std::exception& e) {
        execution->fail("Uncaught exception: " + std::string(e.what()),
                        job.context);
    } catch (...) {
        execution->fail("Uncaught non-exception type.", job.context);
    }

    std::lock_guard guard(currentExecutionFailureMutex);
    if (currentExecutionIsFailed) {
        execution->fail(currentExecutionFailureMessage,
                        currentExecutionFailureContext);
    }
}

void Executor::onWarning(Warning warning, GroupPtr group) {
    decorateWarningWithCurrentTestNotes(warning, std::move(group));
    api->runHooks<ExtensionApi::ON_WARNING>(warning);
}

void Executor::decorateWarningWithCurrentTestNotes(Warning& warning,
                                                   GroupPtr group) {
    if (currentSetUp) {
        warning.addNote("While running setUp", currentSetUp->context);
    }
    if (currentTearDown) {
        warning.addNote("While running tearDown", currentTearDown->context);
    }
    if (currentTest) {
        warning.addNote("While running test " + currentTest->getDescription(),
                        currentTest->getContext());
    }
    while (group != nullptr) {
        warning.addNote("In group " + group->getDescription(),
                        group->getContext());
        group = group->getParentGroup();
    }
}

void Executor::onGroupDiscovered(GroupPtr group) {
    api->runHooks<ExtensionApi::ON_GROUP_DISCOVERED>(std::move(group));
}

void Executor::onTestDiscovered(const Test& test) {
    api->runHooks<ExtensionApi::ON_TEST_DISCOVERED>(test);
}

void Executor::onTestExecutionStart(const Test& test) {
    api->runHooks<ExtensionApi::BEFORE_TEST_EXECUTION>(test);
}

void Executor::onTestExecutionFinish(const Test& test) {
    api->runHooks<ExtensionApi::AFTER_TEST_EXECUTION>(test);
}

}  // namespace mcga::test
