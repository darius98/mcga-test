#ifndef KKTEST_CORE_DRIVER_H_
#define KKTEST_CORE_DRIVER_H_

#include <map>
#include <set>
#include <vector>

#include <core/errors.hpp>
#include <core/executor.hpp>
#include <core/group.hpp>
#include <core/plugin.hpp>

namespace kktest {

class TestingDriver {
public:
    typedef std::function<void(const Test&)> TestHook;
    typedef std::function<void(const Group&)> GroupHook;
    typedef std::function<void()> AfterInitHook;
    typedef std::function<void()> BeforeDestroyHook;
    typedef std::function<void(const ConfigurationError&)> BeforeForceDestroyHook;

    static void setExecutor(Executor* executor);

    static void addBeforeTestHook(const TestHook& hook);
    static void addAfterTestHook(const TestHook& hook);
    static void addBeforeGroupHook(const GroupHook& hook);
    static void addAfterGroupHook(const GroupHook& hook);

    static void addAfterInitHook(const AfterInitHook& hook);
    static void addBeforeDestroyHook(const BeforeDestroyHook& hook);
    static void addBeforeForceDestroyHook(const BeforeForceDestroyHook& hook);

private:
    static TestingDriver* getInstance();
    static TestingDriver* instance;

    static void init();
    static int destroy();
    static void forceDestroy(const ConfigurationError& error);

    static void beforeTestCase();
    static void afterTestCase();

    TestingDriver();

    ~TestingDriver();

    void installPlugins();

    void uninstallPlugins();

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

    void afterTest(Test* test);
    void beforeTest(Test* test);
    void beforeGroup(Group* group);
    void afterGroup(Group* group);

    void markTestFinished(Group* group);
    void markTestStarted(Group* group);
    void markAllTestsStarted(Group* group);

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
    bool useImplicitExecutor = false;
    Group* globalScope = nullptr;
    std::vector<Group*> groupStack = {};
    int currentTestIndex = 0;
    int currentGroupIndex = 0;
    bool allowRegisterHooks = false;

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
