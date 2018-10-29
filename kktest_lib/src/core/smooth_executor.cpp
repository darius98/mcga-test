#include <iostream>

#include "smooth_executor.hpp"

using namespace std;


namespace kktest {

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

void SmoothExecutor::execute(Test* test, Executable func) {
    state = SET_UP;
    executeSetUpsRecursively(test->getParentGroup(), test);
    state = TEST;
    runTest(test, func);
    state = TEAR_DOWN;
    executeTearDownsRecursively(test->getParentGroup(), test);
    state = INACTIVE;
    afterTest(test);
}

void SmoothExecutor::executeSetUpsRecursively(Group* group, Test* test) {
    if (group == nullptr) {
        return;
    }
    executeSetUpsRecursively(group->getParentGroup(), test);

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

void SmoothExecutor::runTest(Test *test, Executable func) {
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
}

void SmoothExecutor::executeTearDownsRecursively(Group* group, Test* test) {
    if (group == nullptr) {
        return;
    }

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

    executeTearDownsRecursively(group->getParentGroup(), test);
}

}
