#ifndef KKTEST_CORE_DRIVER_HPP_
#define KKTEST_CORE_DRIVER_HPP_

#include <functional>
#include <map>
#include <set>
#include <vector>

#include "errors.hpp"
#include "executor.hpp"
#include "group.hpp"
#include "hooks.hpp"
#include "test.hpp"

namespace kktest {

class Driver {
 public:
    static Driver* getInstance();

    static Driver* init(const Hooks& hooks, bool smooth, std::size_t numBoxes);

    Driver(Hooks hooks, bool smooth, std::size_t numBoxes);

    ~Driver();

    int destroy();

    void forceDestroy(const ConfigurationError& error);

    void beforeTestCase(const String& testCaseName);

    void afterTestCase();

    void addGroup(const GroupConfig& config, Executable func);

    void addTest(const TestConfig& config, Executable func);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

 private:
    void afterTest(Test* test);
    void beforeTest(Test* test);
    void beforeGroup(Group* group);
    void afterGroup(Group* group);

    void markTestFinished(Group* group);
    void markTestStarted(Group* group);
    void markAllTestsStarted(Group* group);

    static Driver* instance;

    Hooks hooks;

    std::map<Group*, int> testsInExecution;
    std::set<Group*> groupsPendingFinish;

    Executor* executor = nullptr;
    Group* globalScope = nullptr;
    std::vector<Group*> groupStack = {};
    int currentTestIndex = 0;
    int currentGroupIndex = 0;

    bool failedAnyNonOptionalTest = false;
};

}

#endif
