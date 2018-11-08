#ifndef KKTEST_CORE_EXECUTOR_H_
#define KKTEST_CORE_EXECUTOR_H_

#include <set>

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
    virtual ~Executor() = default;

    bool isDuringTest() const;

    void checkIsInactive(const std::string& methodName) const;

    virtual void execute(Test* test, Executable func, Executable after);

    virtual void finalize();

protected:
    void run(Test* test, Executable func);

private:
    void runSetUpsRecursively(Group* group, Test* test);

    void runTearDownsRecursively(Group* group, Test* test);

    void runTest(Test* test, Executable func);

    State state = State::INACTIVE;
};

}

#endif
