#ifndef KKTEST_CORE_DRIVER_H_
#define KKTEST_CORE_DRIVER_H_

#include <JSON>

#include "group.hpp"
#include "executor.hpp"
#include "plugin.hpp"


namespace kktest {

class TestingDriver: private Pluginable {
public:
    static std::string getBinaryPath();

    static bool isDuringTest();

    static void setExecutor(Executor* executor);

    static void addBeforeTestHook(Executor::TestHook hook);
    static void addAfterTestHook(Executor::TestHook hook);
    static void addBeforeGroupHook(Executor::GroupHook hook);
    static void addAfterGroupHook(Executor::GroupHook hook);

    static void addAfterInitHook(CopyableExecutable hook);
    static void addBeforeDestroyHook(CopyableExecutable hook);

    static autojson::JSON toJSON();

private:
    static TestingDriver* init(const std::string& binaryPath,
                               const std::vector<Plugin*>& plugins);
    static int destroy();

    static TestingDriver* getInstance();
    static TestingDriver* instance;

    TestingDriver(const std::string& _binaryPath,
                  const std::vector<Plugin*>& plugins);

    ~TestingDriver();

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

    std::string binaryPath;
    std::vector<CopyableExecutable> afterInitHooks;
    std::vector<CopyableExecutable> beforeDestroyHooks;
    Group* globalScope;
    std::vector<Group*> groupStack;
    Executor* executor;

friend int main(int argc, char **argv);
friend class Definer;
friend class TestDefiner;
friend class GroupDefiner;
friend class SetUpDefiner;
friend class TearDownDefiner;
friend class ExpectDefiner;
};

}

#endif
