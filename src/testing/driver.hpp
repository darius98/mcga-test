#ifndef KKTEST_TESTING_DRIVER_H_
#define KKTEST_TESTING_DRIVER_H_

#include "group.hpp"
#include "executor.hpp"


namespace kktest {

class TestingDriver {
public:
    static void initGlobal(const std::string& executorName);

    static int destroyGlobal();

    static void generateTestReport(std::ostream& report);

    static bool isDuringTest();

private:
    static TestingDriver* getGlobalDriver();

    explicit TestingDriver(const std::string& executorName);

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

    int getNumFailedTests();

private:
    Group* globalScope;
    std::vector<Group*> groupStack;
    Executor* executor;

friend class Definer;
};

}

#endif
