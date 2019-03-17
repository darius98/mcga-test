#include "kktest/core/driver.hpp"

#include "kktest/core/include/kktest.hpp"
#include "kktest/core/box_executor.hpp"
#include "kktest/core/warning.hpp"

using namespace std;
using namespace std::placeholders;

namespace kktest {

Driver* Driver::instance = nullptr;

Driver* Driver::Instance() {
    return instance;
}

bool Driver::IsInsideTestCase() {
    return instance != nullptr && !instance->groupStack.empty();
}

Driver* Driver::Init(const HooksManager& api,
                     ExecutorType executorType,
                     size_t numBoxes) {
    instance = new Driver(api, executorType, numBoxes);
    instance->runHooks<HooksManager::AFTER_INIT>();
    return instance;
}

void Driver::clean() {
    executor->finalize();
    runHooks<HooksManager::BEFORE_DESTROY>();
}

void Driver::forceDestroy(const ConfigurationError& error) {
    runHooks<HooksManager::BEFORE_FORCE_DESTROY>(error);
}

void Driver::addGroup(GroupConfig config, const Executable& body) {
    if (executor->isActive()) {
        EmitWarning("Called kktest::group() inside a kktest::",
                    executor->stateAsString(),
                    "(). Ignoring...");
        return;
    }

    ++ currentGroupId;
    auto parentGroup = groupStack.empty() ? nullptr : groupStack.back();
    auto group = make_shared<Group>(move(config), parentGroup, currentGroupId);

    groupStack.push_back(group);
    beforeGroup(group);
    try {
        body();
    } catch(const ConfigurationError& e) {
        throw e;
    } catch(const ExpectationFailed& e) {
        throw ConfigurationError(
            "Expectation failed in group \"" + group->getDescription() + "\""
            ": " + e.what());
    } catch(const exception& e) {
        throw ConfigurationError(
            "Exception thrown in group \"" + group->getDescription() + "\""
            ": " + e.what());
    } catch(...) {
        throw ConfigurationError(
            "Non-exception thrown in group \"" + group->getDescription() + "\""
            ".");
    }
    group->setStartedAllTests();
    if (group->finishedAllTests()) {
        afterGroup(group);
    }
    groupStack.pop_back();
}

void Driver::addTest(TestConfig config, Executable body) {
    if (executor->isActive()) {
        EmitWarning("Called kktest::test() inside a kktest::",
                    executor->stateAsString(),
                    "(). Ignoring...");
        return;
    }
    GroupPtr parentGroup = groupStack.back();
    Test test(move(config), move(body), parentGroup, ++ currentTestId);
    parentGroup->addStartedTest();
    beforeTest(test);
    executor->execute(move(test));
}

void Driver::addSetUp(Executable func) {
    if (executor->isActive()) {
        EmitWarning("Called kktest::setUp() inside a kktest::",
                    executor->stateAsString(),
                    "(). Ignoring...");
        return;
    }
    groupStack.back()->addSetUp(move(func));
}

void Driver::addTearDown(Executable func) {
    if (executor->isActive()) {
        EmitWarning("Called kktest::tearDown() inside a kktest::",
                    executor->stateAsString(),
                    "(). Ignoring...");
        return;
    }
    groupStack.back()->addTearDown(move(func));
}

Driver::Driver(HooksManager hooksManager,
               ExecutorType executorType,
               size_t numBoxes): HooksManager(move(hooksManager)) {
    switch (executorType) {
        // TODO(darius98): Try to make executors not take callbacks for what to
        //  do after tests.
        case ExecutorType::SMOOTH_EXECUTOR: {
            this->executor = make_unique<Executor>(
                    /*onTestFinished=*/bind(&Driver::afterTest, this, _1));
            break;
        }
        case ExecutorType::BOXED_EXECUTOR: {
            this->executor = make_unique<BoxExecutor>(
                    /*onTestFinished=*/bind(&Driver::afterTest, this, _1),
                    numBoxes);
            break;
        }
        default: {
            throw ConfigurationError("Driver::Unknown executor type.");
        }
    }
}

void Driver::beforeTest(const Test& test) {
    runHooks<HooksManager::BEFORE_TEST>(test);
}

void Driver::afterTest(const ExecutedTest& test) {
    runHooks<HooksManager::AFTER_TEST>(test);
    GroupPtr parentGroup = test.getGroup();
    parentGroup->addFinishedTest();
    while (parentGroup != nullptr) {
        if (parentGroup->finishedAllTests()) {
            afterGroup(parentGroup);
        }
        parentGroup = parentGroup->getParentGroup();
    }
}

void Driver::beforeGroup(GroupPtr group) {
    runHooks<HooksManager::BEFORE_GROUP>(group);
}

void Driver::afterGroup(GroupPtr group) {
    runHooks<HooksManager::AFTER_GROUP>(group);
}

}
