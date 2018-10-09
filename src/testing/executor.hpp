#ifndef KKTEST_TESTING_EXECUTOR_H_
#define KKTEST_TESTING_EXECUTOR_H_

#include "executable.hpp"
#include "group.hpp"
#include "test.hpp"


namespace kktest {

class Executor {
public:
    explicit Executor(const std::string& name);

    bool isDuringTest() const;

    void checkIsInactive(const std::string& methodName) const;

    void execute(const std::vector<Group*>& groups,
                 Test* test,
                 Executable func);

private:
    void executeLocked(Test* test, int testIndex);

    void executeSimple(const std::vector<Group*>& groups,
                       Test* test,
                       Executable func);

    void executeSetUps(const std::vector<Group*>& groups, Test* test);

    void executeTearDowns(const std::vector<Group*>& groups, Test* test);

    void executeTest(Test* test, Executable func);

    enum ExecutorState {
        INACTIVE = 0,
        SET_UP = 1,
        TEST = 2,
        TEAR_DOWN = 3
    };

    ExecutorState state = ExecutorState::INACTIVE;
    bool copiedBinary = false;
    int testIndex = 0;
    std::string name;
};

}

#endif
