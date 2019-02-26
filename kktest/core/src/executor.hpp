#ifndef KKTEST_CORE_SRC_EXECUTOR_HPP_
#define KKTEST_CORE_SRC_EXECUTOR_HPP_

#include "kktest/core/src/group.hpp"
#include "kktest/core/src/executed_test.hpp"

namespace kktest {

class Executor {
 public:
    typedef std::function<void(const ExecutedTest&)> OnTestFinished;

    static double getTimeTickLengthMs();

    explicit Executor(OnTestFinished onTestFinished);

    virtual ~Executor() = default;

    void checkIsInactive(const std::string& methodName) const;

    virtual void execute(Test&& test);

    virtual void finalize();

 protected:
    ExecutedTest::Info run(GroupPtr group, const Executable& func);

 private:
    void runSetUps(GroupPtr group, ExecutedTest::Info* executionInfo);

    void runTest(const Executable& func, ExecutedTest::Info* executionInfo);

    void runTearDowns(GroupPtr group, ExecutedTest::Info* executionInfo);

 protected:
    OnTestFinished onTestFinished;

 private:
    enum { ACTIVE, INACTIVE } state = INACTIVE;
};

}

#endif
