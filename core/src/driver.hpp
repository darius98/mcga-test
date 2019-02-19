#ifndef KKTEST_CORE_SRC_DRIVER_HPP_
#define KKTEST_CORE_SRC_DRIVER_HPP_

#include <functional>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "core/src/errors.hpp"
#include "core/src/executor.hpp"
#include "core/src/group.hpp"
#include "core/src/hooks.hpp"
#include "core/src/test.hpp"

namespace kktest {

class Driver {
 public:
    static Driver* getInstance();

    static Driver* init(const Hooks& hooks, bool smooth, std::size_t numBoxes);

    Driver(Hooks hooks, bool smooth, std::size_t numBoxes);

    int clean();

    void forceDestroy(const ConfigurationError& error);

    void beforeTestCase(const std::string& testCaseName);

    void afterTestCase();

    void addGroup(GroupConfig&& config, Executable func);

    void addTest(TestConfig&& config, Executable func);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

 private:
    void beforeTest(const Test& test);
    void afterTest(const ExecutedTest& test);
    void beforeGroup(GroupPtr group);
    void afterGroup(GroupPtr group);

    void markTestStarted(GroupPtr group);
    void markAllTestsStarted(GroupPtr group);
    void markTestFinished(GroupPtr test);

    static Driver* instance;

    Hooks hooks;

    std::map<GroupPtr, int> testsInExecution;
    std::set<GroupPtr> groupsPendingFinish;

    std::unique_ptr<Executor> executor;
    std::vector<GroupPtr> groupStack = {};
    int currentTestIndex = 0;
    int groupIndex = 0;

    bool failedAnyNonOptionalTest = false;
};

}

#endif
