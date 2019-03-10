#include "kktest/core/src/driver.hpp"

#include "kktest/core/include/kktest.hpp"
#include "kktest/core/src/box_executor.hpp"

using namespace std;
using namespace std::placeholders;

namespace kktest {

Driver* Driver::instance = nullptr;

Driver* Driver::Instance() {
    if (instance == nullptr) {
        throw ConfigurationError("Driver: getInstance() called before init().");
    }
    return instance;
}

Driver* Driver::Init(const ExtensionApi& api,
                     ExecutorType executorType,
                     size_t numBoxes) {
    if (instance != nullptr) {
        throw ConfigurationError("Driver: init() called a twice.");
    }
    instance = new Driver(api, executorType, numBoxes);
    instance->extensionApi.runHooks<ExtensionApi::AFTER_INIT>();
    return instance;
}

int Driver::clean() {
    extensionApi.runHooks<ExtensionApi::BEFORE_DESTROY>();
    return failedAnyNonOptionalTest ? 1 : 0;
}

void Driver::forceDestroy(const ConfigurationError& error) {
    extensionApi.runHooks<ExtensionApi::BEFORE_FORCE_DESTROY>(error);
}

Driver::Driver(ExtensionApi extensionApi,
               ExecutorType executorType,
               size_t numBoxes): extensionApi(move(extensionApi)) {
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

void Driver::afterTestCase() {
    executor->finalize();
}

void Driver::addGroup(GroupConfig config, const Executable& body) {
    executor->checkIsInactive("group");

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
    executor->checkIsInactive("test");
    GroupPtr parentGroup = groupStack.back();
    Test test(move(config), move(body), parentGroup, ++ currentTestId);
    parentGroup->addStartedTest();
    beforeTest(test);
    executor->execute(move(test));
}

void Driver::addSetUp(Executable func) {
    executor->checkIsInactive("setUp");
    groupStack.back()->addSetUp(move(func));
}

void Driver::addTearDown(Executable func) {
    executor->checkIsInactive("tearDown");
    groupStack.back()->addTearDown(move(func));
}

void Driver::beforeTest(const Test& test) {
    extensionApi.runHooks<ExtensionApi::BEFORE_TEST>(test);
}

void Driver::afterTest(const ExecutedTest& test) {
    failedAnyNonOptionalTest |= (!test.isOptional() && !test.isPassed());
    extensionApi.runHooks<ExtensionApi::AFTER_TEST>(test);
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
    extensionApi.runHooks<ExtensionApi::BEFORE_GROUP>(group);
}

void Driver::afterGroup(GroupPtr group) {
    extensionApi.runHooks<ExtensionApi::AFTER_GROUP>(group);
}

}
