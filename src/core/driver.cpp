#include "driver.hpp"

#include "mcga/test.hpp"
#include "utils.hpp"

namespace mcga::test {

Driver* Driver::Instance() {
    return instance;
}

void Driver::Init(Driver* _instance) {
    instance = _instance;
}

void Driver::Clean() {
    instance->executor->finalize();
    instance = nullptr;
}

Driver::Driver(Executor* executor): executor(executor) {
}

Executor::Type Driver::getExecutorType() const {
    return executor->getType();
}

void Driver::addGroup(GroupConfig config, Executable body) {
    if (!checkMainThreadAndInactive("group", body.context)) {
        return;
    }

    ++currentGroupId;
    GroupPtr parentGroup = groupStack.empty() ? nullptr : groupStack.back();
    GroupPtr group = make_shared<Group>(
      std::move(config), std::move(body.context), parentGroup, currentGroupId);

    groupStack.push_back(group);
    try {
        body();
    } catch (const std::exception& e) {
        emitWarning(Warning(
          "Exception thrown outside a test: " + std::string(e.what()),
          std::nullopt,
          {Warning::Note("Unable to execute remainder of tests in this group.",
                         std::nullopt)}));
    } catch (...) {
        emitWarning(Warning(
          "Non-exception object thrown outside a test.",
          std::nullopt,
          {Warning::Note("Unable to execute remainder of tests in this group.",
                         std::nullopt)}));
    }
    groupStack.pop_back();
}

void Driver::addTest(TestConfig config, Executable body) {
    if (!checkMainThreadAndInactive("test", body.context)) {
        return;
    }
    GroupPtr parentGroup = groupStack.back();
    executor->execute(
      Test(std::move(config), std::move(body), parentGroup, ++currentTestId));
}

void Driver::addSetUp(Executable setUp) {
    if (!checkMainThreadAndInactive("setUp", setUp.context)) {
        return;
    }
    groupStack.back()->addSetUp(std::move(setUp));
}

void Driver::addTearDown(Executable tearDown) {
    if (!checkMainThreadAndInactive("tearDown", tearDown.context)) {
        return;
    }
    groupStack.back()->addTearDown(std::move(tearDown));
}

void Driver::addFailure(Test::ExecutionInfo info) {
    if (!executor->isActive()) {
        emitWarning(
          Warning("Called "
                    + std::string(
                      info.status == Test::ExecutionInfo::FAILED ? "fail" : "skip")
                    + "() outside a test, ignoring.",
                  std::move(info.context)));
        return;
    }
    executor->addFailure(std::move(info));
}

void Driver::addCleanup(Executable cleanup) {
    if (!executor->isActive()) {
        emitWarning(Warning("Called cleanup() outside a test, ignoring.",
                            std::move(cleanup.context)));
        return;
    }
    executor->addCleanup(std::move(cleanup));
}

void Driver::emitWarning(Warning warning) {
    executor->emitWarning(std::move(warning), groupStack.back());
}

bool Driver::checkMainThreadAndInactive(const String& method,
                                        const Context& context) {
    if (executor->isActive()) {
        emitWarning(Warning(
          "Called " + std::string(method.c_str()) + "() inside a "
            + std::string(executor->stateAsString().c_str()) + "(), ignoring.",
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
