#ifndef KKTEST_CORE_TEST_CONTAINER_H_
#define KKTEST_CORE_TEST_CONTAINER_H_

#include <chrono>
#include <string>

#include <kktest_impl/types.hpp>
#include "test.hpp"

namespace kktest {

class TestContainer {
public:
    TestContainer(Test* _test, double _testProcessTimeLimitMs, Executable testFunc);

    bool isTestFinished();

    Test* getTest() const;
    double getTicks() const;
    bool isPassed() const;
    String getFailureMessage() const;

private:
    bool finish(double ticks, bool passed=true, const String& failureMessage="");

    bool killTestProcess();

    Test* test;
    double testProcessTimeLimitMs;
    int testProcessPipeFD;
    pid_t testProcessPID;
    std::chrono::time_point<std::chrono::high_resolution_clock> testProcessStartTime;

    // Results
    double ticks;
    bool passed;
    String failureMessage;
};

}

#endif
