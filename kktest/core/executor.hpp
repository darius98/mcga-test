#pragma once

#include <mutex>

#include "kktest/core/executed_test.hpp"
#include "kktest/core/warning.hpp"

namespace kktest {

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

    std::string stateAsString() const;

    void addFailure(const std::string& failure);

    virtual void execute(Test test);

    virtual void finalize();

    ExecutedTest::Info run(const Test& test);

    virtual void handleWarning(const std::string& message);

 private:
    void runJob(const Executable& job,
                ExecutedTest::Info* execution,
                const std::string& where);

 protected:
    OnTestFinished onTestFinished;
    OnWarning onWarning;

 private:
    enum {
        INACTIVE,
        INSIDE_TEST,
        INSIDE_SET_UP,
        INSIDE_TEAR_DOWN
    } state = INACTIVE;
    int currentTestGroupId = 0;
    int currentTestId = 0;
    std::size_t currentExecutionThreadId;

    std::mutex currentExecutionFailureMutex;
    bool currentExecutionIsFailed;
    std::string currentExecutionFailureMessage;
};

}
