#ifndef KKTEST_CORE_SRC_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_EXECUTOR_HPP_

#include "executable.hpp"
#include "core/src/group.hpp"
#include "core/src/test.hpp"
#include "core/src/test_run.hpp"

namespace kktest {

class Executor {
 public:
    typedef std::function<void(const TestRun&)> OnTestFinished;

    static double getTimeTickLengthMs();

    explicit Executor(OnTestFinished _onTestFinishedCallback);

    virtual ~Executor();

    void checkIsInactive(const std::string& methodName) const;

    virtual void execute(Test&& test, Executable func);

    virtual void finalize();

 protected:
    ExecutionInfo run(GroupPtr group, Executable func);

 private:
    void runSetUps(GroupPtr group, ExecutionInfo* executionInfo);

    void runTest(Executable func, ExecutionInfo* executionInfo);

    void runTearDowns(GroupPtr group, ExecutionInfo* executionInfo);

 protected:
    OnTestFinished onTestFinishedCallback;

 private:
    enum { ACTIVE, INACTIVE } state = INACTIVE;
};

}

#endif
