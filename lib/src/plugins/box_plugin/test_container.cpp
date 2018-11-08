#include <JSON>

#include <unistd.h>

#include "test_container.hpp"

using namespace autojson;
using namespace std;

namespace kktest {

TestContainer::TestContainer(Test *_test,
                             int _testProcessPipeFD,
                             int _testProcessPID,
                             CopyableExecutable _afterTestCallback):
        test(_test),
        testProcessPipeFD(_testProcessPipeFD),
        testProcessPID(_testProcessPID),
        afterTestCallback(move(_afterTestCallback)) {}

bool TestContainer::operator<(const TestContainer &other) const {
    return testProcessPID < other.testProcessPID;
}

bool TestContainer::isTestFinished() const {
    ssize_t numBytesRead = read(testProcessPipeFD,
                                processOutputReadBuffer,
                                PROCESS_READ_BUFFER_SIZE - 1);
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
        close(testProcessPipeFD);
        test->loadFromJSON(JSON::parse(processOutput));
        afterTestCallback();
        return true;
    }
    return false;
}

}
