#ifndef KKTEST_PLUGINS_BOX_PLUGIN_TEST_CONTAINER_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_TEST_CONTAINER_H_

#include <string>

#include <core/executable.hpp>
#include <core/test.hpp>

namespace kktest {

class TestContainer {
private:
    static constexpr const std::size_t PROCESS_READ_BUFFER_SIZE = 1u << 10;

public:
    TestContainer(Test *_test,
                  int _testProcessPipeFD,
                  int _testProcessPID,
                  CopyableExecutable _afterTestCallback);

    bool operator <(const TestContainer& other) const;

    bool isTestFinished() const;

private:
    Test* test = nullptr;
    int testProcessPipeFD;
    pid_t testProcessPID;
    CopyableExecutable afterTestCallback;

    mutable char processOutputReadBuffer[PROCESS_READ_BUFFER_SIZE];
    mutable std::string processOutput;
};

}

#endif
