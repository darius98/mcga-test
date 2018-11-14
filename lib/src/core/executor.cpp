#include <chrono>

#include "executor.hpp"
#include "expectation_failed.hpp"

using namespace std;
using namespace std::chrono;

namespace kktest {

Executor::Executor(): timeTickLengthMs(computeTimeTickLengthFromHardware()) {}

Executor::~Executor() = default;

double Executor::computeTimeTickLengthFromHardware() {
     // TODO: Don't hardcode this, estimate it based on how much a series of computations takes.
    return 1000.0;
}

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

double Executor::getTimeTickLengthMs() const {
    return timeTickLengthMs;
}

void Executor::finalize() {}

void Executor::execute(Test* test, Executable func, Executable after) {
    run(test, func);
    after();
}

void Executor::run(Test* test, Executable func) {
    state = SET_UP;
    auto begin = high_resolution_clock::now();
    pair<string, bool> setUpFailure = runSetUpsRecursively(test->getGroup());
    state = TEST;
    pair<string, bool> testFailure = runTest(func);
    state = TEAR_DOWN;
    pair<string, bool> tearDownFailure = runTearDownsRecursively(test->getGroup());
    auto end = high_resolution_clock::now();
    state = INACTIVE;
    if (setUpFailure.second) {
        setUpFailure = testFailure;
    }
    if (setUpFailure.second) {
        setUpFailure = tearDownFailure;
    }
    setTestExecuted(test,
        /* executionTimeTicks=*/duration_cast<milliseconds>(end - begin).count() / timeTickLengthMs,
        /*             passed=*/setUpFailure.second,
        /*     failureMessage=*/setUpFailure.first
    );
}

void Executor::setTestExecuted(Test* test,
                               double executionTimeTicks,
                               bool passed,
                               const string& failureMessage) {
    test->setExecuted(executionTimeTicks, passed, failureMessage);
}

pair<string, bool> Executor::runSetUpsRecursively(Group* group) {
    if (group == nullptr) {
        return {"", true};
    }
    pair<string, bool> recursiveFailure = runSetUpsRecursively(group->getParentGroup());

    bool failed;
    string failMessage;
    try {
        group->setUp();
        failed = false;
    } catch(const exception& e) {
        failed = true;
        failMessage = "An exception was thrown during the setUp of group '"
                      + group->getFullDescription() + "': " + e.what();
    } catch(...) {
        failed = true;
        failMessage = "A non-exception object was thrown during the setUp of group '"
                      + group->getFullDescription() + "'.";
    }
    if (recursiveFailure.second) {
        return {failMessage, !failed};
    }
    return recursiveFailure;
}

pair<string, bool> Executor::runTest(Executable func) {
    try {
        func();
    } catch(const ExpectationFailed& failure) {
        return {failure.what(), false};
    } catch(const exception& e) {
        return {"An exception was thrown during test: " + string(e.what()), false};
    } catch(...) {
        return {"A non-exception object was thrown during test", false};
    }
    return {"", true};
}

pair<string, bool> Executor::runTearDownsRecursively(Group* group) {
    if (group == nullptr) {
        return {"", true};
    }

    bool failed;
    string failMessage;
    try {
        group->tearDown();
        failed = false;
    } catch(const exception& e) {
        failed = true;
        failMessage = "An exception was thrown during the tearDown of group '"
                      + group->getFullDescription() + "': " + e.what();
    } catch(...) {
        failed = true;
        failMessage = "A non-exception object was thrown during the tearDown of group '"
                      + group->getFullDescription() + "'.";
    }

    pair<string, bool> recursiveFailure = runTearDownsRecursively(group->getParentGroup());
    if (!failed) {
        return recursiveFailure;
    }
    return {failMessage, !failed};
}

}
