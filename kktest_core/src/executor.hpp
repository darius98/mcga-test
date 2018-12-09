#ifndef KKTEST_CORE_SRC_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_EXECUTOR_HPP_

#include <kktest_common/strutil.hpp>
#include <kktest_impl/executable.hpp>
#include "group.hpp"
#include "test.hpp"

namespace kktest {

class Executor {
 private:
    enum State { INACTIVE, ACTIVE };
    typedef const std::function<void(const String&)>& SetFailure;

    static double computeTimeTickLengthFromHardware();

 public:
    typedef std::function<void(Test*)> OnTestFinishedCallback;

    explicit Executor(OnTestFinishedCallback _onTestFinishedCallback);

    virtual ~Executor();

    void checkIsInactive(const String& methodName) const;

    double getTimeTickLengthMs() const;

    virtual void execute(Test* test, Executable func);

    virtual void finalize();

 protected:
    TestExecutionInfo run(Test* test, Executable func);

    std::function<void(Test*)> onTestFinishedCallback;

 private:
    void runSetUpsRecursively(Group* group, SetFailure setFailure);

    void runTearDownsRecursively(Group* group, SetFailure setFailure);

    void runTest(Executable func, SetFailure setFailure);

    State state = State::INACTIVE;
    double timeTickLengthMs;
};

}  // namespace kktest

#endif  // KKTEST_CORE_SRC_EXECUTOR_HPP_
