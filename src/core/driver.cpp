#include "driver.hpp"

#include "config.hpp"
#include "mcga/test.hpp"
#include "utils.hpp"

static mcga::test::Driver* driverInstance = nullptr;

namespace mcga::test {

Driver* Driver::Instance() {
    return driverInstance;
}

void Driver::Init(Driver* _instance) {
    driverInstance = _instance;
}

void Driver::Clean() {
    driverInstance->executor->finalize();
    driverInstance = nullptr;
}

Driver::Driver(ExtensionApi* api, Executor* executor)
        : executor(executor), api(api) {
    executor->setExtensionApi(api);
}

Executor::Type Driver::getExecutorType() const {
    return executor->getType();
}

void Driver::addGroup(GroupConfig config, Executable body) {
    if (!checkMainThreadAndInactive(WarningNoteType::GROUP, body.context)) {
        return;
    }

    ++currentGroupId;
    auto group = Group::make(
      std::move(config), body.context, currentGroup, currentGroupId);
    if (group == nullptr) {
        emitWarning("Failed to allocate storage for group. Increase the number "
                    "of statically allocated groups by changing the "
                    "MCGA_TEST_PRE_ALLOCATED_GROUPS build flag.",
                    body.context);
        return;
    }
    currentGroup = group;
    api->onGroupDiscovered(currentGroup);
#if MCGA_TEST_EXCEPTIONS
    try {
        body();
    } catch (const std::exception& e) {
        emitWarning(
          String::cat("Exception thrown outside a test: ",
                      e.what(),
                      ". Unable to execute remainder of tests in this group."));
    } catch (...) {
        emitWarning("Non-exception object thrown outside a test. Unable to "
                    "execute remainder of tests in this group.");
    }
#else
    body();
#endif
    currentGroup = currentGroup->getParentGroup();
}

void Driver::addTest(TestConfig config, Executable body) {
    if (!checkMainThreadAndInactive(WarningNoteType::TEST, body.context)) {
        return;
    }
    Test test(
      std::move(config), std::move(body), currentGroup, ++currentTestId);
    api->onTestDiscovered(test);
    executor->execute(std::move(test));
}

void Driver::addSetUp(Executable setUp) {
    const auto context = setUp.context;
    if (!checkMainThreadAndInactive(WarningNoteType::SET_UP, context)) {
        return;
    }
    if (!currentGroup->addSetUp(std::move(setUp))) {
        emitWarning("Failed to allocate storage for setUp. Increase the "
                    "number of statically allocated callbacks by changing the "
                    "MCGA_TEST_PRE_ALLOCATED_CALLBACKS build flag.",
                    context);
    }
}

void Driver::addTearDown(Executable tearDown) {
    const auto context = tearDown.context;
    if (!checkMainThreadAndInactive(WarningNoteType::TEAR_DOWN, context)) {
        return;
    }
    if (!currentGroup->addTearDown(std::move(tearDown))) {
        emitWarning("Failed to allocate storage for tearDown. Increase the "
                    "number of statically allocated callbacks by changing the "
                    "MCGA_TEST_PRE_ALLOCATED_CALLBACKS build flag.",
                    context);
    }
}

void Driver::addFailure(Test::ExecutionInfo info) {
    if (!executor->isActive()) {
        emitWarning(info.status == Test::ExecutionInfo::FAILED
                      ? "Called fail() outside a test, ignoring."
                      : "Called skip() outside a test, ignoring.",
                    info.context);
        return;
    }
    executor->addFailure(std::move(info));
}

void Driver::addCleanup(Executable cleanup) {
    const auto context = cleanup.context;

    if (!executor->isActive()) {
        emitWarning("Called cleanup() outside a test, ignoring.", context);
        return;
    }
    if (!executor->addCleanup(std::move(cleanup))) {
        emitWarning("Failed to allocate storage for cleanup. Increase the "
                    "number of statically allocated callbacks by changing the "
                    "MCGA_TEST_PRE_ALLOCATED_CALLBACKS build flag.",
                    context);
    }
}

void Driver::emitWarning(String message, std::optional<Context> context) {
    executor->emitWarning(Warning(std::move(message), std::move(context)),
                          currentGroup);
}

bool Driver::checkMainThreadAndInactive(WarningNoteType method,
                                        const Context& context) {
    if (executor->isActive()) {
        const char* warningMessage = [&] {
            switch (method) {
                case WarningNoteType::TEST:
                    return "Called test() inside a test, ignoring.";
                case WarningNoteType::GROUP:
                    return "Called group() inside a test, ignoring.";
                case WarningNoteType::SET_UP:
                    return "Called setUp() inside a test, ignoring.";
                case WarningNoteType::TEAR_DOWN:
                    return "Called tearDown() inside a test, ignoring.";
                default: return "";
            }
        }();
        emitWarning(warningMessage, context);
        return false;
    }
    if (testingThreadId != current_thread_id()) {
        const char* warningMessage = [&] {
            switch (method) {
                case WarningNoteType::TEST:
                    return "Called test() from a different thread than the "
                           "main testing thread, ignoring.";
                case WarningNoteType::GROUP:
                    return "Called group() from a different thread than the "
                           "main testing thread, ignoring.";
                case WarningNoteType::SET_UP:
                    return "Called setUp() from a different thread than the "
                           "main testing thread, ignoring.";
                case WarningNoteType::TEAR_DOWN:
                    return "Called tearDown() from a different thread than the "
                           "main testing thread, ignoring.";
                default: return "";
            }
        }();
        emitWarning(warningMessage, context);
        return false;
    }
    return true;
}

}  // namespace mcga::test
