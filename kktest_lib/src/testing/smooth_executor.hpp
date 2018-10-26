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
    explicit SmoothExecutor(int testIndexToRun);

    bool isDuringTest() const override;

    void checkIsInactive(const std::string& methodName) const override;

    void finalize() override;

private:
    void execute(Test* test, Executable func) override;

    void executeSetUpsRecursively(Group* group, Test* test);

    void executeTearDownsRecursively(Group* group, Test* test);

    void runTest(Test* test, Executable func);

    State state;
};

}

#endif
