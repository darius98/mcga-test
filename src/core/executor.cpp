#include "executor.hpp"

#include <chrono>

#include "time_tick.hpp"
#include "utils.hpp"
#include "config.hpp"

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
#if MCGA_TEST_EXCEPTIONS
    // We kill the thread on failure if we are in the main testing thread, and
    // we know we catch this exception. This only works if exceptions are
    // enabled. If the user starts his own threads that entertain interruptions,
    // it is their responsibility to make sure his threads die on failure (we
    // have no control).
    if (current_thread_id() == currentExecutionThreadId) {
        throw Interrupt{};
    }
#endif
}

bool Executor::addCleanup(Executable cleanup) {
    return cleanups.push_back(std::move(cleanup));
}

void Executor::execute(Test test) {
    while (true) {
        beforeTestExecution(test);
        if (test.isFinished()) {
            break;
        }
        test.addExecution(run(test), api);
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
    auto startTime = Now();
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
    auto timeTicks = TimeTicksSince(startTime);
    auto info = currentExecution.reset();
    info.timeTicks = timeTicks;
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
#if MCGA_TEST_EXCEPTIONS
    try {
        job();
    } catch (Interrupt& interruption) {
    } catch (const std::exception& e) {
        currentExecution.fail(String::cat("Uncaught exception: ", e.what()));
    } catch (...) {
        currentExecution.fail("Uncaught non-exception type.");
    }
#else
    job();
#endif
    currentJob = nullptr;
}

void Executor::onWarning(Warning warning, GroupPtr group) {
    decorateWarningWithCurrentTestNotes(warning, std::move(group));
    api->onWarning(warning);
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

void Executor::beforeTestExecution(Test& test) {
    while (true) {
        std::optional<Test::ExecutionInfo> execution;
        api->beforeTestExecution(test, execution);
        if (!execution.has_value()) {
            break;
        }
        test.addExecution(*std::move(execution), api);
        if (test.isFinished()) {
            break;
        }
    }
}

SmoothExecutor::SmoothExecutor(): Executor(SMOOTH) {
}

}  // namespace mcga::test
