#include <unistd.h>
#include <sys/wait.h>

#include <JSON>

#include <utils/unescape_characters.hpp>
#include "test_container.hpp"

using namespace autojson;
using namespace kktest::utils;
using namespace std;
using namespace std::chrono;


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

string readStringFromPipe(int pipeFD, int maxReadAttempts=1024) {
    string output;
    char buffer[128];
    for (int i = 0; i < maxReadAttempts; ++ i) {
        ssize_t numBytesRead = read(pipeFD, buffer, 127);
        if (numBytesRead < 0) {
            perror("read");
            exit(errno);
        }
        buffer[numBytesRead] = 0;
        output += buffer;

        // reading is done on encountering '\0'.
        if (numBytesRead > 0 && buffer[numBytesRead - 1] == 0) {
            break;
        }
    }
    return output;
}

}


namespace kktest {

TestContainer::TestContainer(Test *_test,
                             double _testProcessTimeLimitMs,
                             Executable run,
                             CopyableExecutable _afterTestCallback):
        test(_test),
        testProcessTimeLimitMs(_testProcessTimeLimitMs),
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
            {"failureMessage", test->getFailureMessage()},
            {"executionTimeTicks", test->getExecutionTimeTicks()}
        }).stringify(), fd[1]);
        exit(0);
    }
    close(fd[1]);
    testProcessStartTime = high_resolution_clock::now();
}

bool TestContainer::isTestFinished() {
    int wStatus;
    int ret = waitpid(testProcessPID, &wStatus, WNOHANG);
    if (ret < 0) {
        perror("waitpid");
        exit(errno);
    }
    if (ret == 0) {
        auto currentTime = high_resolution_clock::now();
        double elapsedTimeMs = duration_cast<milliseconds>
                                   (currentTime - testProcessStartTime).count();
        if (elapsedTimeMs > testProcessTimeLimitMs) {
            int killStatus = kill(testProcessPID, SIGKILL);
            if (killStatus < 0) {
                if (errno == ESRCH) {
                    // The child might have finished during a context switch.
                    // In this case, return false so we can retry waiting later.
                    return false;
                }
                perror("kill");
                exit(errno);
            }
            test->setExecuted(-1.0);
            test->setFailure("Execution time limit exceeded.");
            close(testProcessPipeFD);
            afterTestCallback();
            return true;
        }
        // The child process (test) did not exit yet.
        return false;
    }
    // Process exited.
    double timeTicks = -1.0;
    if (WIFEXITED(wStatus)) {
        int exitStatus = WEXITSTATUS(wStatus);
        if (exitStatus != 0) {
            test->setFailure("Non-zero exit code: " + to_string(exitStatus));
        } else {
            string processOutput = readStringFromPipe(testProcessPipeFD);
            auto json = JSON::parse(processOutput);
            if (json.type != JSONType::OBJECT ||
                    !json.isBool("passed") ||
                    !json.isReal("executionTimeTicks") ||
                    (!json.get("passed").operator bool() &&
                        !json.isString("failureMessage"))) {
                test->setFailure("Test unexpectedly exited with code 0");
            } else {
                if (!json.get("passed").operator bool()) {
                    test->setFailure(unescapeCharacters(
                            json.get("failureMessage").operator std::string()
                    ));
                }
                timeTicks = json.get("executionTimeTicks").operator double();
            }
        }
    } else if (WIFSIGNALED(wStatus)) {
        test->setFailure("Killed by signal " + to_string(WTERMSIG(wStatus)));
    } else {
        test->setFailure("Unknown error occured.");
    }
    test->setExecuted(timeTicks);
    close(testProcessPipeFD);
    afterTestCallback();
    return true;
}

}
