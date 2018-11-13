#ifndef KKTEST_PLUGINS_BOX_PLUGIN_TEST_CONTAINER_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_TEST_CONTAINER_H_

#include <chrono>
#include <string>

#include <core/executable.hpp>
#include <core/test.hpp>

namespace kktest {

class TestContainer {
public:
    TestContainer(Test* _test,
                  double _testProcessTimeLimitMs,
                  Executable testFunc,
                  std::function<void(double, bool, std::string)> _afterTestCallback);

    bool isTestFinished();

private:
    bool finish(double ticks, bool passed=true, const std::string& failureMessage="");

    bool killTestProcess();

    Test* test;
    double testProcessTimeLimitMs;
    int testProcessPipeFD;
    pid_t testProcessPID;
    std::function<void(double, bool, std::string)> afterTestCallback;
    std::chrono::time_point<std::chrono::high_resolution_clock> testProcessStartTime;
};

}

#endif
