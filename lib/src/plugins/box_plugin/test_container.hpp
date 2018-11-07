#ifndef KKTEST_PLUGINS_BOX_PLUGIN_TEST_CONTAINER_H_
#define KKTEST_PLUGINS_BOX_PLUGIN_TEST_CONTAINER_H_

#include <string>

#include <core/test.hpp>

namespace kktest {

class SubprocessCaller {
public:
    SubprocessCaller();

    void setFD(int fd);

    bool poll();

    std::string getOutput() const;

private:

};

class TestContainer {
public:
    void fill(Test* _test, int fd);

    bool tryFinalize();

    Test* getTest() const;

    std::string getOutput() const;

private:
    bool poll();

    Test* test = nullptr;

    bool available = true;
    bool outputAvailable = false;

    int processFileDescriptor;
    char processOutputReadBuffer[32];
    std::string processOutput;
};

}

#endif
