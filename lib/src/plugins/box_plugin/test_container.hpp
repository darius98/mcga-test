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
    TestContainer(Test* _test,
                  Executable testFunc,
                  CopyableExecutable _afterTestCallback);

    ~TestContainer();

    bool isTestFinished();

private:
    Test* test;
    int testProcessPipeFD;
    pid_t testProcessPID;
    CopyableExecutable afterTestCallback;

    char processOutputReadBuffer[PROCESS_READ_BUFFER_SIZE];
    std::string processOutput;
};

}

#endif
