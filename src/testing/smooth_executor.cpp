#include <iostream>

#include "smooth_executor.hpp"

using namespace std;


namespace kktest {

SmoothExecutor::SmoothExecutor(int testIndexToRun):
    Executor(testIndexToRun), state(INACTIVE) {}

bool SmoothExecutor::isDuringTest() const {
    return state == TEST;
}

void SmoothExecutor::checkIsInactive(const string& methodName) const {
    if (state == TEST) {
        throw runtime_error("Cannot call " + methodName + " within test!");
    }
    if (state == SET_UP) {
        throw runtime_error("Cannot call " + methodName + " within setUp!");
    }
    if (state == TEAR_DOWN) {
        throw runtime_error("Cannot call " + methodName + " within tearDown!");
    }
}

void SmoothExecutor::finalize() {}

void SmoothExecutor::execute(const vector<Group*>& groups,
                             Test* test,
                             Executable func) {
    executeSetUps(groups, test);
    executeTest(test, func);
    executeTearDowns(groups, test);
    afterTest(test);
}

void SmoothExecutor::executeSetUps(const vector<Group*>& groups, Test* test) {
    state = SET_UP;
    for (Group* group: groups) {
        bool failed;
        string failMessage;
        try {
            group->setUp();
            failed = false;
        } catch(const exception& e) {
            failed = true;
            failMessage = "An exception was thrown during the "
                          "setUp of group '" + group->getFullDescription()
                          + "': " + e.what();
        } catch(...) {
            failed = true;
            failMessage = "A non-exception object was thrown during the "
                          "setUp of group '" + group->getFullDescription() + "'.";
        }
        if (failed) {
            test->setFailure(failMessage);
        }
    }
    state = INACTIVE;
}

void SmoothExecutor::executeTest(Test *test, Executable func) {
    state = TEST;
    try {
        func();
    } catch(const ExpectationFailed& failure) {
        test->setFailure(failure);
    } catch(const exception& e) {
        test->setFailure(
            "An exception was thrown during test: " + string(e.what())
        );
    } catch(...) {
        test->setFailure("A non-exception object was thrown during test");
    }
    state = INACTIVE;
}

void SmoothExecutor::executeTearDowns(const vector<Group*>& groups,
                                      Test* test) {
    state = TEAR_DOWN;
    for (int i = (int)groups.size() - 1; i >= 0; -- i) {
        Group* group = groups[i];
        bool failed;
        string failMessage;
        try {
            group->tearDown();
            failed = false;
        } catch(const exception& e) {
            failed = true;
            failMessage = "An exception was thrown during the "
                          "tearDown of group '" + group->getFullDescription()
                          + "': " + e.what();
        } catch(...) {
            failed = true;
            failMessage = "A non-exception object was thrown during the "
                          "tearDown of group '" + group->getFullDescription()
                          + "'.";
        }
        if (failed) {
            test->setFailure(failMessage);
        }
    }
    state = INACTIVE;
}

}
