#ifndef KKTEST_TESTING_SMOOTH_EXECUTOR_H_
#define KKTEST_TESTING_SMOOTH_EXECUTOR_H_

#include "executor.hpp"


namespace kktest {

class SmoothExecutor: public Executor {
private:
    enum State {
        INACTIVE = 0,
        SET_UP = 1,
        TEST = 2,
        TEAR_DOWN = 3
    };

public:
    SmoothExecutor(int testIndexToRun, bool verbose);

    bool isDuringTest() const override;

    void checkIsInactive(const std::string& methodName) const override;

    void finalize() override;

private:
    void execute(const std::vector<Group*>& groups,
                 Test* test,
                 Executable func) override;

    void executeSetUps(const std::vector<Group*>& groups, Test* test);

    void executeTearDowns(const std::vector<Group*>& groups, Test* test);

    void executeTest(Test* test, Executable func);

    State state;
};

}

#endif
