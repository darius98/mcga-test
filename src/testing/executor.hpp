#ifndef KKTEST_TESTING_EXECUTOR_H_
#define KKTEST_TESTING_EXECUTOR_H_

#include "testing/executable.hpp"
#include "testing/group.hpp"
#include "testing/test.hpp"


namespace kktest {

class TestingDriver;

class Executor {
public:
    Executor();

    bool isDuringTest() const;

    void checkIsInactive(const std::string& methodName) const;

    void execute(const std::vector<Group*>& groups,
                 Test* test,
                 Executable func);

    void executeGroup(Group* group, Executable func);

private:
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
    int testIndex = 0;
};

}

#endif
