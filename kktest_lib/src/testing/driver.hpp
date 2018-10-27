#ifndef KKTEST_TESTING_DRIVER_H_
#define KKTEST_TESTING_DRIVER_H_

#include "group.hpp"
#include "executor.hpp"
#include "test_logger.hpp"
#include "test_pipe.hpp"


namespace kktest {

class TestingDriver {
public:
    static void init(const std::string& binaryPath);

    static int destroy();

    static bool isDuringTest();

    static void addBeforeTestHook(Executor::TestHook hook);
    static void addAfterTestHook(Executor::TestHook hook);
    static void addBeforeGroupHook(Executor::GroupHook hook);
    static void addAfterGroupHook(Executor::GroupHook hook);

    static TestingDriver* getInstance();

private:
    static TestingDriver* instance;

    explicit TestingDriver(const std::string& binaryPath);

public:
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

private:
    Group* globalScope;
    std::vector<Group*> groupStack;
    Executor* executor;
    TestLogger* testLogger = nullptr;
    TestPipe* testPipe = nullptr;
};

}

#endif
