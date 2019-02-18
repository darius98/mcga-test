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
    void afterTest(const TestRun& testRun);
    void beforeGroup(Group* group);
    void afterGroup(Group* group);

    void markTestStarted(Group* group);
    void markAllTestsStarted(Group* group);
    void markTestFinished(Group* test);

    static Driver* instance;

    Hooks hooks;

    std::map<Group*, int> testsInExecution;
    std::set<Group*> groupsPendingFinish;

    std::unique_ptr<Executor> executor;
    std::vector<Group*> groupStack = {};
    int currentTestIndex = 0;
    int currentGroupIndex = 0;

    bool failedAnyNonOptionalTest = false;
};

}

#endif
