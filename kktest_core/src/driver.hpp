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

class TestingDriverHooks {
public:
    enum Type {
        AFTER_INIT = 0,
        BEFORE_GROUP = 1,
        AFTER_GROUP = 2,
        BEFORE_TEST = 3,
        AFTER_TEST = 4,
        BEFORE_DESTROY = 5,
        BEFORE_FORCE_DESTROY = 6,
    };

    template<Type t, class H>
    void addHook(const H& hook) {
        std::get<t>(hooks).push_back(hook);
    }

    template<Type t, class... Args>
    void runHooks(const Args... args) {
        for (const auto& hook: std::get<t>(hooks)) {
            hook(args...);
        }
    }

private:
    std::tuple<
        std::vector<AfterInitHook>,
        std::vector<GroupHook>,
        std::vector<GroupHook>,
        std::vector<TestHook>,
        std::vector<TestHook>,
        std::vector<BeforeDestroyHook>,
        std::vector<BeforeForceDestroyHook>
    > hooks;
};

class TestingDriver {
public:
    static TestingDriver* getInstance();

    static void init(const std::vector<Plugin*>& plugins);
    static int destroy();
    static void forceDestroy(const ConfigurationError& error);

    static void beforeTestCase();
    static void afterTestCase();

    template<TestingDriverHooks::Type t, class H>
    static void addHook(const H& hook) {
        TestingDriver* driver = getInstance();
        if (!driver->allowRegisterHooks) {
            throw KKTestLibraryImplementationError("Hook added outside Plugin::install()!");
        }
        driver->hookManager.addHook<t, H>(hook);
    }

    explicit TestingDriver(const std::vector<Plugin*>& _plugins);

    ~TestingDriver();

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
