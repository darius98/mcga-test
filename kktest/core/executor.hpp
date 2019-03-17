#pragma once

#include "kktest/core/executed_test.hpp"

namespace kktest {

class Executor {
 public:
    typedef std::function<void(const ExecutedTest&)> OnTestFinished;
    typedef std::function<void(const std::string&)> OnWarning;

    virtual ~Executor() = default;

    void setOnTestFinishedCallback(OnTestFinished _onTestFinished);

    void setOnWarningCallback(OnWarning _onWarning);

    bool isActive() const;

    std::string stateAsString() const;

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
};

}
