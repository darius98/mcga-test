#include <unistd.h>

#include <functional>

#include "test_container.hpp"

using namespace std;

namespace kktest {

void TestContainer::fill(Test* _test, int fd, Executable _after) {
    if (!available) {
        throw runtime_error("Trying to run in un-available box!");
    }

    test = _test;
    outputAvailable = false;
    available = false;
    processOutput.clear();
    processFileDescriptor = fd;
    after = _after;
}

bool TestContainer::tryFinalize() {
    if (test == nullptr) {
        return true;
    }
    if (!poll()) {
        return false;
    }
    test = nullptr;
    return true;
}

Test* TestContainer::getTest() const {
    return test;
}

string TestContainer::getOutput() const {
    if (!outputAvailable) {
        throw runtime_error("Output not available!");
    }
    return processOutput;
}

void TestContainer::executeAfter() const {
    after();
}

bool TestContainer::poll() {
    if (available) {
        return true;
    }

    int numBytesRead = read(processFileDescriptor, processOutputReadBuffer, 31);
    if (numBytesRead < 0) {
        perror("read");
        exit(errno);
    }
    if (numBytesRead == 0) {
        return false;
    }
    processOutputReadBuffer[numBytesRead] = 0;
    processOutput += processOutputReadBuffer;
    if (processOutputReadBuffer[numBytesRead - 1] == 0) {
        // reading is done on encountering zero.
        close(processFileDescriptor);
        outputAvailable = true;
        available = true;
    }
    return available;
}

}
