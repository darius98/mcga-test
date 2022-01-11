#include "executor.hpp"

#include <chrono>
#include <stdexcept>
#include <thread>

#include "time_tick.hpp"
#include "utils.hpp"

namespace mcga::test {

class Interrupt : public std::exception {
  public:
    Test::ExecutionInfo info;

    explicit Interrupt(Test::ExecutionInfo info): info(std::move(info)) {
    }

    [[nodiscard]] const char* what() const noexcept override {
        return info.message.c_str();
    }
};

Executor::Executor(ExtensionApi* api): api(api) {
}

bool Executor::isActive() const {
    return state != INACTIVE;
}

String Executor::stateAsString() const {
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

void Executor::addFailure(Test::ExecutionInfo info) {
    // We only kill the thread on failure if we are in the main testing thread
    // and we know we catch this exception.
    if (current_thread_id() == currentExecutionThreadId) {
        // TODO: Abort when exceptions are disabled (in non-smooth execution,
        //  inform the test runner process of the failure before aborting).
        throw Interrupt(std::move(info));
    }

    // If the user starts his own threads that entertain failures, it is his
    // responsibility to make sure his threads die on failure (we have no
    // control).
    std::lock_guard guard(currentExecutionStatusMutex);
    currentExecution.merge(std::move(info));
}

void Executor::addCleanup(Executable cleanup) {
    currentExecutionCleanups.push_back(std::move(cleanup));
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
    std::vector<GroupPtr> groups = test.getGroupStack();
    std::vector<GroupPtr>::iterator it;
    currentTest = &test;
    state = INSIDE_SET_UP;
    Test::ExecutionInfo info;
    auto startTime = std::chrono::high_resolution_clock::now();
    // Execute setUp()-s, in the order of the group stack.
    for (it = groups.begin(); info.isPassed() && it != groups.end(); ++it) {
        (*it)->forEachSetUp([&](const Executable& setUp) {
            currentSetUp = &setUp;
            runJob(setUp, &info);
            currentSetUp = nullptr;
            // If a setUp() fails, do not execute the rest.
            return info.status == Test::ExecutionInfo::PASSED;
        });
    }
    state = INSIDE_TEST;
    if (info.isPassed()) {
        // Only run the test if all setUp()-s passed without exception.
        runJob(test.getBody(), &info);
    }
    state = INSIDE_TEAR_DOWN;
    for (const auto& cleanup: currentExecutionCleanups) {
        currentCleanup = &cleanup;
        runJob(cleanup, &info);
        currentCleanup = nullptr;
    }
    currentExecutionCleanups.clear();
    // Execute tearDown()-s in reverse order, from where setUp()-s stopped.
    for (--it; it + 1 != groups.begin(); --it) {
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
    state = INACTIVE;
    currentTest = nullptr;
    return info;
}

void Executor::emitWarning(Warning warning, GroupPtr group) {
    onWarning(std::move(warning), std::move(group));
}

void Executor::runJob(const Executable& job, Test::ExecutionInfo* execution) {
    currentExecutionThreadId = current_thread_id();
    currentExecution
      = Test::ExecutionInfo{.status = Test::ExecutionInfo::PASSED,
                            .message = "OK",
                            .context = std::nullopt};

    try {
        job();
    } catch (Interrupt& interruption) {
        execution->merge(std::move(interruption.info));
    } catch (const std::exception& e) {
        execution->fail("Uncaught exception: " + std::string(e.what()),
                        job.context);
    } catch (...) {
        execution->fail("Uncaught non-exception type.", job.context);
    }

    std::lock_guard guard(currentExecutionStatusMutex);
    execution->merge(std::move(currentExecution));
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
    if (currentCleanup) {
        warning.addNote("While running cleanup", currentCleanup->context);
    }
    if (currentTearDown) {
        warning.addNote("While running tearDown", currentTearDown->context);
    }
    if (currentTest) {
        warning.addNote("While running test "
                          + std::string(currentTest->getDescription().c_str()),
                        currentTest->getContext());
    }
    while (group != nullptr) {
        warning.addNote("In group "
                          + std::string(group->getDescription().c_str()),
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
