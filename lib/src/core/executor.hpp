#ifndef KKTEST_CORE_EXECUTOR_H_
#define KKTEST_CORE_EXECUTOR_H_

#include <core/executable.hpp>
#include "group.hpp"
#include "test.hpp"


namespace kktest {

class Executor {
private:
    enum State {
        INACTIVE = 0,
        SET_UP = 1,
        TEST = 2,
        TEAR_DOWN = 3
    };

public:
    Executor();

    virtual ~Executor();

    bool isDuringTest() const;

    void checkIsInactive(const std::string& methodName) const;

    double getTimeTickLengthMs() const;

    virtual void execute(Test* test, Executable func, Executable after);

    virtual void finalize();

protected:
    void run(Test* test, Executable func);

private:
    std::pair<std::string, bool> runSetUpsRecursively(Group* group);

    std::pair<std::string, bool> runTearDownsRecursively(Group* group);

    std::pair<std::string, bool> runTest(Executable func);

    State state = State::INACTIVE;
    double timeTickLengthMs;
};

}

#endif
