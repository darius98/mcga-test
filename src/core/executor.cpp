#include "executor.hpp"

#include <chrono>

#include "time_tick.hpp"
#include "utils.hpp"

namespace mcga::test {

struct Interrupt {};

Executor::Executor(Type type): type(type) {
}

void Executor::setExtensionApi(ExtensionApi* api_) {
    api = api_;
}

ExtensionApi* Executor::getExtensionApi() const {
    return api;
}

bool Executor::isActive() const {
    return state != INACTIVE;
}

void Executor::finalize() {
}

void Executor::addFailure(Test::ExecutionInfo info) {
    currentExecution.merge(std::move(info));
    // We kill the thread on failure if we are in the main testing thread, and
    // we know we catch this exception.
    // If the user starts his own threads that entertain interruptions, it is
    // their responsibility to make sure his threads die on failure (we have no
    // control).
    if (current_thread_id() == currentExecutionThreadId) {
        // TODO: Abort when exceptions are disabled (in non-smooth execution,
        //  inform the test runner process of the failure before aborting).
        throw Interrupt{};
    }
}

void Executor::addCleanup(Executable cleanup) {
    cleanups.push_back(std::move(cleanup));
}

void Executor::execute(Test test) {
    while (true) {
        addHooksExecutions(test);
        if (test.isFinished()) {
            break;
        }
        test.addExecution(run(test));
        api->runHooks<ExtensionApi::AFTER_TEST_EXECUTION>(test);
    }
}

Executor::Type Executor::getType() const {
    return type;
}

GroupPtr Executor::runSetUps(GroupPtr group) {
    if (group == nullptr) {
        return nullptr;
    }
    if (group->hasParentGroup()) {
        auto ret = runSetUps(group->getParentGroup());
        if (!currentExecution.isPassed()) {
            return ret;
        }
    }
    group->forEachSetUp([&](const Executable& setUp) {
        runJob(setUp);
        // If a setUp() fails, do not execute the rest.
        return currentExecution.isPassed();
    });
    return group;
}

Test::ExecutionInfo Executor::run(const Test& test) {
    currentTest = &test;
    currentExecutionThreadId = current_thread_id();

    state = INSIDE_SET_UP;
    auto startTime = std::chrono::high_resolution_clock::now();
    // Execute setUp()-s, in the order of the group stack.
    auto lastGroup = runSetUps(test.getGroup());
    state = INSIDE_TEST;
    if (currentExecution.isPassed()) {
        // Only run the test if all setUp()-s passed without exception.
        runJob(test.getBody());
    }
    state = INSIDE_CLEANUP;
    for (const auto& cleanup: cleanups) {
        runJob(cleanup);
    }
    cleanups.clear();
    state = INSIDE_TEAR_DOWN;
    // Execute tearDown()-s in reverse order, from where setUp()-s stopped.
    while (lastGroup != nullptr) {
        lastGroup->forEachTearDown([&](const Executable& tearDown) {
            runJob(tearDown);
            return true;
        });
        lastGroup = lastGroup->getParentGroup();
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    auto info = currentExecution.reset();
    info.timeTicks = NanosecondsToTimeTicks(endTime - startTime);
    if (info.timeTicks > test.getTimeTicksLimit()) {
        info.fail("Execution timed out.");
    }
    currentTest = nullptr;
    state = INACTIVE;
    return info;
}

void Executor::emitWarning(Warning warning, GroupPtr group) {
    onWarning(std::move(warning), std::move(group));
}

void Executor::runJob(const Executable& job) {
    currentJob = &job;
    try {
        job();
    } catch (Interrupt& interruption) {
    } catch (const std::exception& e) {
        currentExecution.fail("Uncaught exception: " + std::string(e.what()),
                              job.context);
    } catch (...) {
        currentExecution.fail("Uncaught non-exception type.", job.context);
    }
    currentJob = nullptr;
}

void Executor::onWarning(Warning warning, GroupPtr group) {
    decorateWarningWithCurrentTestNotes(warning, std::move(group));
    api->runHooks<ExtensionApi::ON_WARNING>(warning);
}

void Executor::decorateWarningWithCurrentTestNotes(Warning& warning,
                                                   GroupPtr group) {
    if (state == INSIDE_SET_UP && currentJob) {
        warning.addNote(WarningNoteType::SET_UP, "", currentJob->context);
    }
    if (state == INSIDE_CLEANUP && currentJob) {
        warning.addNote(WarningNoteType::CLEANUP, "", currentJob->context);
    }
    if (state == INSIDE_TEAR_DOWN && currentJob) {
        warning.addNote(WarningNoteType::TEAR_DOWN, "", currentJob->context);
    }
    if (currentTest) {
        warning.addNote(WarningNoteType::TEST,
                        currentTest->getDescription(),
                        currentTest->getContext());
    }
    while (group != nullptr) {
        warning.addNote(WarningNoteType::GROUP,
                        group->getDescription().c_str(),
                        group->getContext());
        group = group->getParentGroup();
    }
}

void Executor::addHooksExecutions(Test& test) {
    while (true) {
        std::optional<Test::ExecutionInfo> execution;
        api->runHooks<ExtensionApi::BEFORE_TEST_EXECUTION>(test, execution);
        if (!execution.has_value()) {
            break;
        }
        test.addExecution(*std::move(execution));
        api->runHooks<ExtensionApi::AFTER_TEST_EXECUTION>(test);
        if (test.isFinished()) {
            break;
        }
    }
}

SmoothExecutor::SmoothExecutor(): Executor(SMOOTH) {
}

}  // namespace mcga::test
