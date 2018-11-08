#include <iostream>
#include <JSON>

#include <unistd.h>

#include <utils/unescape_characters.hpp>
#include "test_container.hpp"

using namespace autojson;
using namespace kktest::utils;
using namespace std;


namespace {

void writeStringToPipe(const std::string& output, int pipeFD) {
    size_t bytesToWrite = output.size() + 1;
    const char* bytes = output.c_str();
    int written = 0;
    while (bytesToWrite > 0) {
        ssize_t blockSize = write(pipeFD, bytes + written, bytesToWrite);
        if (blockSize < 0) {
            perror("write");
            exit(errno);
        }
        bytesToWrite -= blockSize;
        written += blockSize;
    }
}

}


namespace kktest {

TestContainer::TestContainer(Test *_test,
                             Executable run,
                             CopyableExecutable _afterTestCallback):
        test(_test),
        afterTestCallback(move(_afterTestCallback)) {
    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(errno);
    }
    testProcessPipeFD = fd[0];
    testProcessPID = fork();
    if (testProcessPID < 0) {
        perror("fork");
        exit(errno);
    }
    if (testProcessPID == 0) { // child
        close(testProcessPipeFD);
        run();
        writeStringToPipe(JSON(map<string, JSON>{
            {"passed", test->isPassed()},
            {"failureMessage", test->getFailureMessage()}
        }).stringify(), fd[1]);
        exit(0);
    }
    close(fd[1]);
}

TestContainer::~TestContainer() {
    close(testProcessPipeFD);
    auto json = JSON::parse(processOutput);
    test->setExecuted();
    if (!json.get("passed").operator bool()) {
        test->setFailure(unescapeCharacters(
                json.get("failureMessage").operator std::string()
        ));
    }
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
