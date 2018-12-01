#ifndef KKTEST_CORE_DRIVER_H_
#define KKTEST_CORE_DRIVER_H_

#include <functional>
#include <map>
#include <set>
#include <vector>

#include "errors.hpp"
#include "executor.hpp"
#include "group.hpp"
#include "extension_api_impl.hpp"

namespace kktest {

class TestingDriver {
public:
    static TestingDriver* getInstance();

    static TestingDriver* init(const TestingDriverHooks& hooks,
                               bool flagBoxed,
                               int argumentNumBoxes);

    explicit TestingDriver(TestingDriverHooks hooks, bool flagBoxed, int argumentNumBoxes);

    ~TestingDriver();

    int destroy();

    void forceDestroy(const ConfigurationError& error);

    void beforeTestCase();

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

    static TestingDriver* instance;

    TestingDriverHooks hookManager;

    std::map<Group*, int> testsInExecutionPerGroup;
    std::set<Group*> groupsWithAllTestsStarted;

    Executor* executor = nullptr;
    Group* globalScope = nullptr;
    std::vector<Group*> groupStack = {};
    int currentTestIndex = 0;
    int currentGroupIndex = 0;

    bool failedAnyNonOptionalTest = false;
};

}

#endif
