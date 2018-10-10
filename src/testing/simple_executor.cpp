#include <iostream>

#include "simple_executor.hpp"

using namespace std;


namespace kktest {

SimpleExecutor::SimpleExecutor(int _testIndexToRun):
        testIndexToRun(_testIndexToRun), state(State::INACTIVE) {}

bool SimpleExecutor::isDuringTest() const {
    return state == State::TEST;
}

void SimpleExecutor::checkIsInactive(const string& methodName) const {
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

void SimpleExecutor::execute(const vector<Group*>& groups,
                             Test* test,
                             Executable func) {
    currentTestIndex += 1;
    if (testIndexToRun == 0 || testIndexToRun == currentTestIndex) {
        executeSetUps(groups, test);
        executeTest(test, func);
        executeTearDowns(groups, test);
    }
    if (testIndexToRun == currentTestIndex) {
        if (test->isFailed()) {
            cout << test->getFailureMessage();
        }
        exit(test->isFailed());
    }
}

void SimpleExecutor::executeSetUps(const vector<Group*>& groups, Test* test) {
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

void SimpleExecutor::executeTest(Test *test, Executable func) {
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

void SimpleExecutor::executeTearDowns(const vector<Group*>& groups,
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
