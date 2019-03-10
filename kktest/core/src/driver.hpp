#pragma once

#include "kktest/core/src/errors.hpp"
#include "kktest/core/src/executor.hpp"
#include "kktest/core/src/extension_api.hpp"

namespace kktest {

enum ExecutorType {
    SMOOTH_EXECUTOR,
    BOXED_EXECUTOR,
};

class Driver {
 private:
    static Driver* instance;

 public:
    static Driver* Instance();

    static Driver* Init(const ExtensionApi& api,
                        ExecutorType executorType,
                        std::size_t numBoxes);

    Driver(ExtensionApi hooks, ExecutorType executorType, std::size_t numBoxes);

    int clean();

    void forceDestroy(const ConfigurationError& error);

    void beforeTestCase(const std::string& testCaseName);

    void afterTestCase();

    void addGroup(GroupConfig&& config, const Executable& func);

    void addTest(TestConfig&& config, Executable func);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

 private:
    void beforeTest(const Test& test);
    void afterTest(const ExecutedTest& test);
    void beforeGroup(GroupPtr group);
    void afterGroup(GroupPtr group);

    ExtensionApi extensionApi;

    std::unique_ptr<Executor> executor;
    std::vector<GroupPtr> groupStack = {};
    int currentTestIndex = 0;
    int groupIndex = 0;

    bool failedAnyNonOptionalTest = false;
};

}
