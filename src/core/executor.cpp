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
        throw ExpectationFailed(failure, std::move(context));
    }

    // If the user starts his own threads that entertain failures, it is his
    // responsibility to make sure his threads die on failure (we have no
    // control)
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
    currentTestId = test.getId();
    state = INSIDE_SET_UP;
    Test::ExecutionInfo info;
    auto startTime = std::chrono::high_resolution_clock::now();
    std::vector<GroupPtr> testGroupStack = test.getGroupStack();
    std::vector<GroupPtr>::iterator it;
    // Execute setUp()-s, in the order of the group stack.
    for (it = testGroupStack.begin(); it != testGroupStack.end(); ++it) {
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
        runJob([&test] { test.run(); }, &info, "test");
        --it;
    }
    state = INSIDE_TEAR_DOWN;
    // Execute tearDown()-s in reverse order, from where setUp()-s stopped.
    for (; it + 1 != testGroupStack.begin(); --it) {
        runJob([it] { (*it)->tearDown(); },
               &info,
               "tearDown of group \"" + (*it)->getDescription() + "\"");
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto nanosecondsElapsed
      = duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
    info.timeTicks = 1.0 * nanosecondsElapsed / GetTimeTickLength().count();
    if (info.timeTicks > test.getTimeTicksLimit()) {
        info.fail("Execution timed out.");
    }
    state = INACTIVE;
    return info;
}

void Executor::emitWarning(const std::string& message, int groupId) {
    if (isActive()) {
        onWarning(Warning(message, groupId, currentTestId));
    }
}

void Executor::runJob(const Executable& job,
                      Test::ExecutionInfo* execution,
                      const std::string& where) {
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
        execution->fail("Uncaught exception in " + where + ": " + e.what());
    } catch (...) {
        execution->fail("Uncaught non-exception type in " + where + "\".");
    }

    std::lock_guard guard(currentExecutionFailureMutex);
    if (currentExecutionIsFailed) {
        execution->fail(currentExecutionFailureMessage,
                        currentExecutionFailureContext);
    }
}

void Executor::onWarning(const Warning& warning) {
    api->runHooks<ExtensionApi::ON_WARNING>(warning);
}

void Executor::onTestExecutionStart(const Test& test) {
    api->runHooks<ExtensionApi::ON_TEST_EXECUTION_START>(test);
}

void Executor::onTestExecutionFinish(const Test& test) {
    api->runHooks<ExtensionApi::ON_TEST_EXECUTION_FINISH>(test);
}

}  // namespace mcga::test
