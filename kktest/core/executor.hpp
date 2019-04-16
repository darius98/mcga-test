#pragma once

#include <mutex>

#include "kktest/core/executed_test.hpp"
#include "kktest/core/warning.hpp"

namespace mcga::test {

class Executor {
  public:
    enum Type {
        SMOOTH,
        BOXED,
    };

    typedef std::function<void(const ExecutedTest&)> OnTestFinished;
    typedef std::function<void(const Warning&)> OnWarning;

    virtual ~Executor() = default;

    void setOnTestFinishedCallback(OnTestFinished _onTestFinished);

    void setOnWarningCallback(OnWarning _onWarning);

    bool isActive() const;

    virtual Type getType() const;

    virtual void execute(Test test);

    virtual void finalize();

    virtual void emitWarning(const std::string& message);

    std::string stateAsString() const;

    void addFailure(const std::string& failure);

    ExecutedTest::Info run(const Test& test);

  private:
    void runJob(const Executable& job,
                ExecutedTest::Info* execution,
                const std::string& where);

  protected:
    OnTestFinished onTestFinished;
    OnWarning onWarning;

  private:
    enum State { INACTIVE, INSIDE_TEST, INSIDE_SET_UP, INSIDE_TEAR_DOWN };

    State state = INACTIVE;
    int currentTestGroupId = 0;
    int currentTestId = 0;
    std::size_t currentExecutionThreadId;

    std::mutex currentExecutionFailureMutex;
    bool currentExecutionIsFailed;
    std::string currentExecutionFailureMessage;
};

}  // namespace mcga::test
