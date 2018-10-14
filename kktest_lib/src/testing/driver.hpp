#ifndef KKTEST_TESTING_DRIVER_H_
#define KKTEST_TESTING_DRIVER_H_

#include "group.hpp"
#include "executor.hpp"
#include "test_logger.hpp"


namespace kktest {

class TestingDriver {
public:
    static void init(const std::string& binaryPath);

    static int destroy();

    static bool isDuringTest();

    static void addAfterTestHook(Executor::Hook);

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
};

}

#endif
