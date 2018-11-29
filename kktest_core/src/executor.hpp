#ifndef KKTEST_CORE_EXECUTOR_H_
#define KKTEST_CORE_EXECUTOR_H_

#include <kktest_impl/executable.hpp>
#include "group.hpp"
#include "test.hpp"

namespace kktest {

class Executor {
private:
    enum State { INACTIVE, ACTIVE };
    typedef const std::function<void(const std::string&)>& SetFailureType;

    static double computeTimeTickLengthFromHardware();

public:
    Executor();

    virtual ~Executor();

    void checkIsInactive(const std::string& methodName) const;

    double getTimeTickLengthMs() const;

    virtual void execute(Test* test, Executable func);

    virtual void finalize();

    void onTestFinished(const std::function<void(Test*)>& _onTestFinishedCallback);

protected:
    void run(Test* test, Executable func);

    void setTestExecuted(Test* test,
                         double executionTimeTicks,
                         bool passed,
                         const std::string& failureMessage);

private:
    void runSetUpsRecursively(Group* group, SetFailureType setFailure);

    void runTearDownsRecursively(Group* group, SetFailureType setFailure);

    void runTest(Executable func, SetFailureType setFailure);

    State state = State::INACTIVE;
    double timeTickLengthMs;
    std::function<void(Test*)> onTestFinishedCallback;
};

}

#endif
