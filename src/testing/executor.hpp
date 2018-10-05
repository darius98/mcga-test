#ifndef KKTEST_TESTING_EXECUTOR_H_
#define KKTEST_TESTING_EXECUTOR_H_

#include "testing/executable.hpp"
#include "testing/group.hpp"
#include "testing/test.hpp"


namespace kktest {

class TestingDriver;

class Executor {
public:
    void executeSetUps(const std::vector<Group*>& groups, Test* test);

    void executeTearDowns(const std::vector<Group*>& groups, Test* test);

    void executeTest(Test* test, Executable func);

    void executeGroup(Group* group, Executable func);
};

}

#endif
