#pragma once

#include "kktest/core/src/errors.hpp"
#include "kktest/core/src/executor.hpp"
#include "kktest/core/src/hooks_manager.hpp"

namespace kktest {

enum ExecutorType {
    SMOOTH_EXECUTOR,
    BOXED_EXECUTOR,
};

class Driver: public HooksManager {
 private:
    static Driver* instance;

 public:
    static Driver* Instance();

    static Driver* Init(const HooksManager& api,
                        ExecutorType executorType,
                        std::size_t numBoxes);

    static bool IsInsideTestCase();

    int clean();

    void forceDestroy(const ConfigurationError& error);

    void addGroup(GroupConfig config, const Executable& body);

    void addTest(TestConfig config, Executable body);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

 private:
    Driver(HooksManager hooks, ExecutorType executorType, std::size_t numBoxes);

    void beforeTest(const Test& test);
    void afterTest(const ExecutedTest& test);
    void beforeGroup(GroupPtr group);
    void afterGroup(GroupPtr group);

    std::unique_ptr<Executor> executor;
    std::vector<GroupPtr> groupStack = {};
    int currentTestId = 0;
    int currentGroupId = 0;

    bool failedAnyNonOptionalTest = false;
};

}
