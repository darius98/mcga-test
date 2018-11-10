#include <chrono>

#include "executor.hpp"

using namespace std;
using namespace std::chrono;

namespace kktest {

bool Executor::isDuringTest() const {
    return state == TEST;
}

void Executor::checkIsInactive(const string& methodName) const {
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

void Executor::finalize() {}

void Executor::execute(Test* test, Executable func, Executable after) {
    run(test, func);
    after();
}

void Executor::run(Test* test, Executable func) {
    state = SET_UP;
    auto begin = high_resolution_clock::now();
    runSetUpsRecursively(test->getParentGroup(), test);
    state = TEST;
    runTest(test, func);
    state = TEAR_DOWN;
    runTearDownsRecursively(test->getParentGroup(), test);
    auto end = high_resolution_clock::now();
    state = INACTIVE;
    test->setExecuted(duration_cast<milliseconds>(end - begin).count() / timeTickLengthMs);
    if (test->getExecutionTimeTicks() > test->getConfig().timeTicksLimit) {
        test->setFailure("Execution time limit exceeded.");
    }
}

void Executor::runSetUpsRecursively(Group* group, Test* test) {
    if (group == nullptr) {
        return;
    }
    runSetUpsRecursively(group->getParentGroup(), test);

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

void Executor::runTest(Test *test, Executable func) {
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

void Executor::runTearDownsRecursively(Group* group, Test* test) {
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

    runTearDownsRecursively(group->getParentGroup(), test);
}

}
