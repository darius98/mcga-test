#include <iostream>

#include "smooth_executor.hpp"

using namespace std;


namespace kktest {

SmoothExecutor::SmoothExecutor(int testIndexToRun, bool verbose):
    Executor(testIndexToRun, verbose), state(State::INACTIVE) {}

bool SmoothExecutor::isDuringTest() const {
    return state == State::TEST;
}

void SmoothExecutor::checkIsInactive(const string& methodName) const {
    if (state == State::TEST) {
        throw runtime_error("Cannot call " + methodName + " within test!");
    }
    if (state == State::SET_UP) {
        throw runtime_error("Cannot call " + methodName + " within setUp!");
    }
    if (state == State::TEAR_DOWN) {
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
    enqueueTestForLogging(test);
    test->updateGroups();
}

void SmoothExecutor::executeSetUps(const vector<Group*>& groups, Test* test) {
    state = State::SET_UP;
    for (Group* g: groups) {
        bool failed;
        string failMessage;
        try {
            g->setUp();
            failed = false;
        } catch(const exception& e) {
            failed = true;
            failMessage = "An exception was thrown during the "
                          "setUp of group '" + g->getFullDescription()
                          + "': " + e.what();
        } catch(...) {
            failed = true;
            failMessage = "A non-exception object was thrown during the "
                          "setUp of group '" + g->getFullDescription() + "'.";
        }
        if (failed) {
            test->setFailure(failMessage);
        }
    }
    state = State::INACTIVE;
}

void SmoothExecutor::executeTest(Test *test, Executable func) {
    state = State::TEST;
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
    state = State::INACTIVE;
}

void SmoothExecutor::executeTearDowns(const vector<Group*>& groups,
                                      Test* test) {
    state = State::TEAR_DOWN;
    for (int i = (int)groups.size() - 1; i >= 0; -- i) {
        Group* g = groups[i];
        bool failed;
        string failMessage;
        try {
            g->tearDown();
            failed = false;
        } catch(const exception& e) {
            failed = true;
            failMessage = "An exception was thrown during the "
                          "tearDown of group '" + g->getFullDescription()
                          + "': " + e.what();
        } catch(...) {
            failed = true;
            failMessage = "A non-exception object was thrown during the "
                          "tearDown of group '" + g->getFullDescription()
                          + "'.";
        }
        if (failed) {
            test->setFailure(failMessage);
        }
    }
    state = State::INACTIVE;
}

}
