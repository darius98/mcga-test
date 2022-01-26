#include "driver.hpp"

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
        : api(api), executor(executor) {
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
    currentGroup = Group::make(
      std::move(config), body.context, currentGroup, currentGroupId);
    executor->getExtensionApi()->onGroupDiscovered(currentGroup);
    try {
        body();
    } catch (const std::exception& e) {
        emitWarning("Exception thrown outside a test: " + std::string(e.what())
                    + ". Unable to execute remainder of tests in this group.");
    } catch (...) {
        emitWarning("Non-exception object thrown outside a test. Unable to "
                    "execute remainder of tests in this group.");
    }
    currentGroup = currentGroup->getParentGroup();
}

void Driver::addTest(TestConfig config, Executable body) {
    if (!checkMainThreadAndInactive(WarningNoteType::TEST, body.context)) {
        return;
    }
    Test test(
      std::move(config), std::move(body), currentGroup, ++currentTestId);
    executor->getExtensionApi()->onTestDiscovered(test);
    executor->execute(std::move(test));
}

void Driver::addSetUp(Executable setUp) {
    if (!checkMainThreadAndInactive(WarningNoteType::SET_UP, setUp.context)) {
        return;
    }
    currentGroup->addSetUp(std::move(setUp));
}

void Driver::addTearDown(Executable tearDown) {
    if (!checkMainThreadAndInactive(WarningNoteType::TEAR_DOWN,
                                    tearDown.context)) {
        return;
    }
    currentGroup->addTearDown(std::move(tearDown));
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
    if (!executor->isActive()) {
        emitWarning("Called cleanup() outside a test, ignoring.",
                    cleanup.context);
        return;
    }
    executor->addCleanup(std::move(cleanup));
}

void Driver::emitWarning(String message, std::optional<Context> context) {
    executor->emitWarning(Warning(std::move(message), context), currentGroup);
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
