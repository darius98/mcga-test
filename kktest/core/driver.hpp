#pragma once

#include "kktest/core/errors.hpp"
#include "kktest/core/executor.hpp"
#include "kktest/core/hooks_manager.hpp"

namespace kktest {

enum ExecutorType {
    SMOOTH_EXECUTOR,
    BOXED_EXECUTOR,
};

class Driver: public HooksManager {
 private:
    static inline Driver* instance = nullptr;

 public:
    static Driver* Instance();

    static Driver* Init(const HooksManager& api,
                        ExecutorType executorType,
                        std::size_t numBoxes);

    Driver(HooksManager hooks, Executor* executor);

    void clean();

    void addGroup(GroupConfig config, const Executable& body);

    void addTest(TestConfig config, Executable body);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

    void addFailure(const std::string& failure);

 private:
    void emitWarning(const std::string& message);

    void beforeTest(const Test& test);
    void afterTest(const ExecutedTest& test);
    void beforeGroup(GroupPtr group);
    void afterGroup(GroupPtr group);

    std::unique_ptr<Executor> executor;
    std::vector<GroupPtr> groupStack = {};
    int currentTestId = 0;
    int currentGroupId = 0;
};

}
