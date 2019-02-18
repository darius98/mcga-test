#ifndef KKTEST_CORE_SRC_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_EXECUTOR_HPP_

#include "core/include/kktest_impl/executable.hpp"
#include "core/src/group.hpp"
#include "core/src/test.hpp"
#include "core/src/test_run.hpp"

namespace kktest {

class Executor {
 private:
    enum State { INACTIVE, ACTIVE };

    typedef const std::function<void(const std::string&)>& SetFailure;

 public:
    typedef std::function<void(const TestRun&)> OnTestFinished;

    static double getTimeTickLengthMs();

    explicit Executor(OnTestFinished _onTestFinishedCallback);

    virtual ~Executor();

    void checkIsInactive(const std::string& methodName) const;

    virtual void execute(Test&& test, Executable func);

    virtual void finalize();

 protected:
    TestRun run(Test&& test, Executable func);

 private:
    void runSetUpsRecursively(GroupPtr group, SetFailure setFailure);

    void runTearDownsRecursively(GroupPtr group, SetFailure setFailure);

    void runTest(Executable func, SetFailure setFailure);

 protected:
    OnTestFinished onTestFinishedCallback;

 private:
    State state = State::INACTIVE;
};

}

#endif
