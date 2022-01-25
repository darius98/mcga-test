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

Driver::Driver(Executor* executor): executor(executor) {}

Executor::Type Driver::getExecutorType() const {
    return executor->getType();
}

void Driver::addGroup(GroupConfig config, Executable body) {
    if (!checkMainThreadAndInactive("group", body.context)) {
        return;
    }

    ++currentGroupId;
    GroupPtr parentGroup = currentGroup;
    GroupPtr group = Group::make(
      std::move(config), body.context, parentGroup, currentGroupId);

    currentGroup = group;
    executor->getExtensionApi()->runHooks<ExtensionApi::ON_GROUP_DISCOVERED>(group);
    try {
        body();
    } catch (const std::exception& e) {
        emitWarning(
          Warning("Exception thrown outside a test: " + std::string(e.what()),
                  std::nullopt)
            .addNote("Unable to execute remainder of tests in this group.",
                     std::nullopt));
    } catch (...) {
        emitWarning(
          Warning("Non-exception object thrown outside a test.", std::nullopt)
            .addNote("Unable to execute remainder of tests in this group.",
                     std::nullopt));
    }
    currentGroup = currentGroup->getParentGroup();
}

void Driver::addTest(TestConfig config, Executable body) {
    if (!checkMainThreadAndInactive("test", body.context)) {
        return;
    }
    Test test(std::move(config), std::move(body), currentGroup, ++currentTestId);
    executor->getExtensionApi()->runHooks<ExtensionApi::ON_TEST_DISCOVERED>(test);
    executor->execute(std::move(test));
}

void Driver::addSetUp(Executable setUp) {
    if (!checkMainThreadAndInactive("setUp", setUp.context)) {
        return;
    }
    currentGroup->addSetUp(std::move(setUp));
}

void Driver::addTearDown(Executable tearDown) {
    if (!checkMainThreadAndInactive("tearDown", tearDown.context)) {
        return;
    }
    currentGroup->addTearDown(std::move(tearDown));
}

void Driver::addFailure(Test::ExecutionInfo info) {
    if (!executor->isActive()) {
        emitWarning(Warning(info.status == Test::ExecutionInfo::FAILED
                              ? "Called fail() outside a test, ignoring."
                              : "Called skip() outside a test, ignoring.",
                            info.context));
        return;
    }
    executor->addFailure(std::move(info));
}

void Driver::addCleanup(Executable cleanup) {
    if (!executor->isActive()) {
        emitWarning(Warning("Called cleanup() outside a test, ignoring.",
                            cleanup.context));
        return;
    }
    executor->addCleanup(std::move(cleanup));
}

void Driver::emitWarning(Warning warning) {
    executor->emitWarning(std::move(warning), currentGroup);
}

bool Driver::checkMainThreadAndInactive(const String& method,
                                        const Context& context) {
    if (executor->isActive()) {
        emitWarning(Warning("Called " + std::string(method.c_str())
                              + "() inside a " + executor->stateAsString()
                              + "(), ignoring.",
                            context));
        return false;
    }
    if (testingThreadId != current_thread_id()) {
        emitWarning(
          Warning("Called " + std::string(method.c_str())
                    + "() from a different thread than the main testing "
                      "thread, ignoring.",
                  context));
        return false;
    }
    return true;
}

}  // namespace mcga::test
