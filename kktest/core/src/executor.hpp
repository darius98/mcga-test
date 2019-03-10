#pragma once

#include "kktest/core/src/executed_test.hpp"

namespace kktest {

class Executor {
 public:
    typedef std::function<void(const ExecutedTest&)> OnTestFinished;

    static double GetTimeTickLengthMs();

    explicit Executor(OnTestFinished onTestFinished);

    virtual ~Executor() = default;

    bool isActive() const;

    std::string stateAsString() const;

    virtual void execute(Test test);

    virtual void finalize();

    ExecutedTest::Info run(const Test& test);

 private:
    void runJob(const Executable& job,
                ExecutedTest::Info* execution,
                const std::string& where);

 protected:
    OnTestFinished onTestFinished;

 private:
    enum {
        INACTIVE,
        INSIDE_TEST,
        INSIDE_SET_UP,
        INSIDE_TEAR_DOWN
    } state = INACTIVE;
};

}
