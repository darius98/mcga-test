#ifndef KKTEST_CORE_DRIVER_H_
#define KKTEST_CORE_DRIVER_H_

#include "group.hpp"
#include "executor.hpp"
#include "plugin.hpp"


namespace kktest {

class TestingDriver: private Pluginable {
public:
    typedef std::function<void(Test*)> TestHook;
    typedef std::function<void(Group*)> GroupHook;

    static void setExecutor(Executor* executor);

    static void addBeforeTestHook(TestHook hook);
    static void addAfterTestHook(TestHook hook);
    static void addBeforeGroupHook(GroupHook hook);
    static void addAfterGroupHook(GroupHook hook);

    static void addAfterInitHook(CopyableExecutable hook);
    static void addBeforeDestroyHook(CopyableExecutable hook);

private:
    static TestingDriver* getInstance();
    static TestingDriver* instance;

    static void init(const std::vector<Plugin*>& plugins);
    static int destroy();

    TestingDriver(const std::vector<Plugin*>& plugins);

    ~TestingDriver() override;

    void addGroup(std::string description,
                  std::string file,
                  int line,
                  Executable func);

    void addTest(std::string description,
                 std::string file,
                 int line,
                 Executable func);

    void addSetUp(Executable func);

    void addTearDown(Executable func);

    void afterTest(Test* test) const;
    void beforeTest(Test* test) const;
    void beforeGroup(Group* group) const;
    void afterGroup(Group* group) const;

    // Hooks
    std::vector<CopyableExecutable> afterInitHooks;
    std::vector<TestHook> beforeTestHooks;
    std::vector<TestHook> afterTestHooks;
    std::vector<GroupHook> beforeGroupHooks;
    std::vector<GroupHook> afterGroupHooks;
    std::vector<CopyableExecutable> beforeDestroyHooks;

    Group* globalScope;
    std::vector<Group*> groupStack;
    Executor* executor;
    bool useImplicitExecutor = true;
    int currentTestIndex = 0;
    int currentGroupIndex = 0;
    mutable int numFailedTests = 0;

friend int main(int argc, char** argv);
friend class TestDefiner;
friend class GroupDefiner;
friend class SetUpDefiner;
friend class TearDownDefiner;
friend class ExpectDefiner;
};

}

#endif
