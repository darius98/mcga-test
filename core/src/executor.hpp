#ifndef KKTEST_CORE_SRC_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_EXECUTOR_HPP_

#include "executable.hpp"
#include "core/src/group.hpp"
#include "core/src/test.hpp"

namespace kktest {

class Executor {
 public:
    typedef std::function<void(const ExecutedTest&)> OnTestFinished;

    static double getTimeTickLengthMs();

    explicit Executor(OnTestFinished _onTestFinishedCallback);

    virtual ~Executor();

    void checkIsInactive(const std::string& methodName) const;

    virtual void execute(Test&& test, Executable func);

    virtual void finalize();

 protected:
    ExecutedTest::Info run(GroupPtr group, Executable func);

 private:
    void runSetUps(GroupPtr group, ExecutedTest::Info* executionInfo);

    void runTest(Executable func, ExecutedTest::Info* executionInfo);

    void runTearDowns(GroupPtr group, ExecutedTest::Info* executionInfo);

 protected:
    OnTestFinished onTestFinishedCallback;

 private:
    enum { ACTIVE, INACTIVE } state = INACTIVE;
};

}

#endif
