#ifndef KKTEST_CORE_SRC_TEST_CONTAINER_HPP_
#define KKTEST_CORE_SRC_TEST_CONTAINER_HPP_

#include <chrono>
#include <string>

#include <kktest_common/interproc.hpp>
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
    bool finish(double ticks, bool passed = true, const String& failureMessage = "");

    bool killTestProcess();

    Test* test;
    double testProcessTimeLimitMs;
    interproc::PipeReader* testProcessPipe;
    interproc::SubprocessHandler* testProcessHandler;
    std::chrono::time_point<std::chrono::high_resolution_clock> testProcessStartTime;

    // Results
    double ticks;
    bool passed;
    String failureMessage;
};

}  // namespace kktest

#endif  // KKTEST_CORE_SRC_TEST_CONTAINER_HPP_
