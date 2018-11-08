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

TestContainer::~TestContainer() {
    close(testProcessPipeFD);
    test->loadFromJSON(JSON::parse(processOutput));
    afterTestCallback();
}

bool TestContainer::isTestFinished() {
    ssize_t numBytesRead = read(testProcessPipeFD,
                                processOutputReadBuffer,
                                PROCESS_READ_BUFFER_SIZE - 1);
    if (numBytesRead < 0) {
        perror("read");
        exit(errno);
    }
    processOutputReadBuffer[numBytesRead] = 0;
    processOutput += processOutputReadBuffer;

    // reading is done on encountering '\0'.
    return numBytesRead > 0 && processOutputReadBuffer[numBytesRead - 1] == 0;
}

}
