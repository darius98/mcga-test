#ifndef KKTEST_PLUGINS_BOX_PLUGIN_TEST_CONTAINER_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_TEST_CONTAINER_H_

#include <string>

#include <core/executable.hpp>
#include <core/test.hpp>

namespace kktest {

class TestContainer {
public:
    void fill(Test* _test, int fd, Executable _after);

    bool tryFinalize();

    Test* getTest() const;

    std::string getOutput() const;

    void executeAfter() const;

private:
    bool poll();

    Test* test = nullptr;

    CopyableExecutable after;

    bool available = true;
    bool outputAvailable = false;

    int processFileDescriptor;
    char processOutputReadBuffer[32];
    std::string processOutput;
};

}

#endif
