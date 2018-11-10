#ifndef KKTEST_PLUGINS_BOX_PLUGIN_TEST_CONTAINER_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_TEST_CONTAINER_H_

#include <string>

#include <core/executable.hpp>
#include <core/test.hpp>

namespace kktest {

class TestContainer {
public:
    TestContainer(Test* _test,
                  Executable testFunc,
                  CopyableExecutable _afterTestCallback);

    bool isTestFinished();

private:
    Test* test;
    int testProcessPipeFD;
    pid_t testProcessPID;
    CopyableExecutable afterTestCallback;
};

}

#endif
