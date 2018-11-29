#ifndef KKTEST_CORE_DRIVER_H_
#define KKTEST_CORE_DRIVER_H_

#include <functional>
#include <map>
#include <set>
#include <vector>

#include "errors.hpp"
#include "executor.hpp"
#include "group.hpp"
#include "plugin_api_impl.hpp"

namespace kktest {

class TestingDriver {
public:
    static TestingDriver* getInstance();

    static TestingDriver* init(const std::vector<Plugin*>& plugins);

    explicit TestingDriver(const std::vector<Plugin*>& _plugins);

    ~TestingDriver();

    int destroy();

    void forceDestroy(const ConfigurationError& error);

    void beforeTestCase();

    void afterTestCase();

    void addGroup(const GroupConfig& config, Executable func);

    void addTest(const TestConfig& config, Executable func);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

    template<TestingDriverHooks::Type t, class H>
    void addHook(const H& hook) {
        if (!allowRegisterHooks) {
            throw KKTestLibraryImplementationError("Hook added outside Plugin::install()!");
        }
        hookManager.addHook<t, H>(hook);
    }

private:
    void installPlugins();

    void uninstallPlugins();

    void afterTest(Test* test);
    void beforeTest(Test* test);
    void beforeGroup(Group* group);
    void afterGroup(Group* group);

    void markTestFinished(Group* group);
    void markTestStarted(Group* group);
    void markAllTestsStarted(Group* group);

    static TestingDriver* instance;

    std::vector<Plugin*> plugins;

    TestingDriverHooks hookManager;

    std::map<Group*, int> testsInExecutionPerGroup;
    std::set<Group*> groupsWithAllTestsStarted;

    Executor* executor = nullptr;
    Group* globalScope = nullptr;
    std::vector<Group*> groupStack = {};
    int currentTestIndex = 0;
    int currentGroupIndex = 0;
    bool allowRegisterHooks = false;

    bool failedAnyNonOptionalTest = false;
};

}

#endif
