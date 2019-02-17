#ifndef KKTEST_CORE_SRC_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_EXECUTOR_HPP_

#include "common/string/src/string.hpp"
#include "core/include/kktest_impl/executable.hpp"
#include "core/src/group.hpp"
#include "core/src/test.hpp"

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

}

#endif
