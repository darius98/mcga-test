#ifndef KKTEST_CORE_DRIVER_H_
#define KKTEST_CORE_DRIVER_H_

#include <functional>
#include <map>
#include <set>
#include <vector>

#include <kktest_impl/plugin_api.hpp>
#include "errors.hpp"
#include "executor.hpp"
#include "group.hpp"

namespace kktest {

class TestingDriver {
public:
    static void addPlugin(Plugin* plugin);

    static TestingDriver* getInstance();

    static void init();
    static int destroy();
    static void forceDestroy(const ConfigurationError& error);

    static void beforeTestCase();
    static void afterTestCase();

    TestingDriver();

    ~TestingDriver();

    template<class H>
    void addHook(H h, std::vector<H>& hs) {
        if (!allowRegisterHooks) {
            throw KKTestLibraryImplementationError("Hook added outside Plugin::install()!");
        }
        hs.emplace_back(std::move(h));
    }

    void addGroup(const GroupConfig& config, Executable func);

    void addTest(const TestConfig& config, Executable func);

    void addSetUp(Executable func, const std::string& file, int line);

    void addTearDown(Executable func, const std::string& file, int line);

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

    static std::vector<Plugin*>* plugins;
    static TestingDriver* instance;

    // Hooks
    std::vector<AfterInitHook> afterInitHooks;
    std::vector<TestHook> beforeTestHooks;
    std::vector<TestHook> afterTestHooks;
    std::vector<GroupHook> beforeGroupHooks;
    std::vector<GroupHook> afterGroupHooks;
    std::vector<BeforeDestroyHook> beforeDestroyHooks;
    std::vector<BeforeForceDestroyHook> beforeForceDestroyHooks;

    std::map<Group*, int> testsInExecutionPerGroup;
    std::set<Group*> groupsWithAllTestsStarted;

    Executor* executor = nullptr;
    Group* globalScope = nullptr;
    std::vector<Group*> groupStack = {};
    int currentTestIndex = 0;
    int currentGroupIndex = 0;
    bool allowRegisterHooks = false;

    bool failedAnyNonOptionalTest = false;

friend void addBeforeTestHook(const TestHook& hook);
friend void addAfterTestHook(const TestHook& hook);
friend void addBeforeGroupHook(const GroupHook& hook);
friend void addAfterGroupHook(const GroupHook& hook);
friend void addAfterInitHook(const AfterInitHook& hook);
friend void addBeforeDestroyHook(const BeforeDestroyHook& hook);
friend void addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook);
};

}

#endif
