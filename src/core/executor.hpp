#pragma once

#include <mutex>

#include "disallow_copy_and_move.hpp"
#include "executed_test.hpp"
#include "warning.hpp"

namespace mcga::test {

class Executor {
  public:
    enum Type {
        SCAN,
        SMOOTH,
        BOXED,
    };

    using OnTestFinished = std::function<void(const ExecutedTest&)>;
    using OnWarning = std::function<void(const Warning&)>;

    Executor() = default;

    MCGA_DISALLOW_COPY_AND_MOVE(Executor);

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
    std::size_t currentExecutionThreadId = 0;

    std::mutex currentExecutionFailureMutex;
    bool currentExecutionIsFailed = false;
    std::string currentExecutionFailureMessage;
};

}  // namespace mcga::test
