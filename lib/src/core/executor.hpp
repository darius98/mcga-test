#ifndef KKTEST_CORE_EXECUTOR_H_
#define KKTEST_CORE_EXECUTOR_H_

#include <core/executable.hpp>
#include "group.hpp"
#include "test.hpp"


namespace kktest {

class Executor {
private:
    enum State {
        INACTIVE = 0,
        SET_UP = 1,
        TEST = 2,
        TEAR_DOWN = 3
    };

    static double computeTimeTickLengthFromHardware();

public:
    Executor();

    virtual ~Executor();

    bool isDuringTest() const;

    void checkIsInactive(const std::string& methodName, const std::string& file, int line) const;

    double getTimeTickLengthMs() const;

    virtual void execute(Test* test, Executable func, Executable after);

    virtual void finalize();

protected:
    void run(Test* test, Executable func);

    void setTestExecuted(Test* test,
                         double executionTimeTicks,
                         bool passed,
                         const std::string& failureMessage);

private:
    bool runSetUpsRecursively(Group* group, std::string* failureMessage);

    bool runTearDownsRecursively(Group* group, std::string* failureMessage);

    bool runTest(Executable func, std::string* failureMessage);

    State state = State::INACTIVE;
    double timeTickLengthMs;
};

}

#endif
