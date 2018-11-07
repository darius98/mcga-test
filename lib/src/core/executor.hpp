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
    typedef std::function<void(Test*)> TestHook;
    typedef std::function<void(Group*)> GroupHook;

    Executor();

    virtual ~Executor();

    bool isDuringTest() const;

    void checkIsInactive(const std::string& methodName) const;

    virtual void finalize();

    void copyHooks(Executor* other);

    void executeTest(Test* test, Executable func);

    void addBeforeTestHook(TestHook hook);
    void addAfterTestHook(TestHook hook);
    void addBeforeGroupHook(GroupHook hook);
    void addAfterGroupHook(GroupHook hook);

    void afterTest(Test* test) const;
    void beforeTest(Test* test) const;
    void beforeGroup(Group* group) const;
    void afterGroup(Group* group) const;

protected:
    void run(Test* test, Executable func);

private:
    virtual void execute(Test* test, Executable func);

    void runSetUpsRecursively(Group* group, Test* test);

    void runTearDownsRecursively(Group* group, Test* test);

    void runTest(Test* test, Executable func);

    std::vector<TestHook> beforeTestHooks;
    std::vector<TestHook> afterTestHooks;
    std::vector<GroupHook> beforeGroupHooks;
    std::vector<GroupHook> afterGroupHooks;

    State state = State::INACTIVE;
};

}

#endif
