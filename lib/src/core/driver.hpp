#ifndef KKTEST_CORE_DRIVER_H_
#define KKTEST_CORE_DRIVER_H_

#include "errors.hpp"
#include "executor.hpp"
#include "group.hpp"
#include "plugin.hpp"

namespace kktest {

class TestingDriver {
public:
    typedef std::function<void(Test*)> TestHook;
    typedef std::function<void(Group*)> GroupHook;
    typedef std::function<void()> AfterInitHook;
    typedef std::function<void()> BeforeDestroyHook;
    typedef std::function<void(const ConfigurationError&)> BeforeForceDestroyHook;

    static void setExecutor(Executor* executor);

    static void addBeforeTestHook(TestHook hook);
    static void addAfterTestHook(TestHook hook);
    static void addBeforeGroupHook(GroupHook hook);
    static void addAfterGroupHook(GroupHook hook);

    static void addAfterInitHook(AfterInitHook hook);
    static void addBeforeDestroyHook(BeforeDestroyHook hook);
    static void addBeforeForceDestroyHook(BeforeForceDestroyHook hook);

private:
    static TestingDriver* getInstance();
    static TestingDriver* instance;

    static void init();
    static int destroy();
    static void forceDestroy(const ConfigurationError& error);

    TestingDriver();

    ~TestingDriver();

    void installPlugins();

    void uninstallPlugins();

    void addGroup(const GroupConfig& config, Executable func);

    void addTest(const TestConfig& config, Executable func);

    void addSetUp(Executable func, const std::string& file, int line);

    void addTearDown(Executable func, const std::string& file, int line);

    void afterTest(Test* test) const;
    void beforeTest(Test* test) const;
    void beforeGroup(Group* group) const;
    void afterGroup(Group* group) const;

    // Hooks
    std::vector<AfterInitHook> afterInitHooks;
    std::vector<TestHook> beforeTestHooks;
    std::vector<TestHook> afterTestHooks;
    std::vector<GroupHook> beforeGroupHooks;
    std::vector<GroupHook> afterGroupHooks;
    std::vector<BeforeDestroyHook> beforeDestroyHooks;
    std::vector<BeforeForceDestroyHook> beforeForceDestroyHooks;

    Group* globalScope;
    std::vector<Group*> groupStack;
    Executor* executor;
    bool useImplicitExecutor = true;
    int currentTestIndex = 0;
    int currentGroupIndex = 0;

    bool failedAnyNonOptionalTest = false;

friend int main();
friend class TestDefiner;
friend class GroupDefiner;
friend class SetUpDefiner;
friend class TearDownDefiner;
friend class ExpectDefiner;
};

}

#endif
