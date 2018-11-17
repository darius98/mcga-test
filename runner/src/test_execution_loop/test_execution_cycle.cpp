#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <cstdlib>

#include <stdexcept>

#include "test_execution_cycle.hpp"

using namespace messaging;
using namespace std;

namespace kktest_runner {

TestExecutionCycle::TestExecutionCycle(
            const string& _testPath,
            int _maxParallelTests,
            const function<void(const KKTestCaseInfo&)>& _onTestCallback):
        started(false),
        testPath(_testPath),
        maxParallelTests(_maxParallelTests),
        onTestCallback(_onTestCallback),
        pipeWithTestProcess(nullptr),
        testProcessPID(0),
        finished(false) {
    info.testExecutablePath = testPath;
}

bool TestExecutionCycle::isStarted() const {
    return started;
}

void TestExecutionCycle::start() {
    if (started) {
        throw runtime_error("Trying to start the same test cycle twice!");
    }
    started = true;
    for (int i = 0; i < 10; ++ i) {
        pipeName += static_cast<char>(rand() % 26 + 97);
    }
    int pipeCreateStatus = mkfifo(pipeName.c_str(), 0666);
    if (pipeCreateStatus < 0) {
        perror("mkfifo");
        exit(errno);
    }
    pipeFD = open(pipeName.c_str(), O_RDONLY | O_NONBLOCK);
    if (pipeFD < 0) {
        perror("open");
        exit(errno);
    }

    pipeWithTestProcess = new InputPipe(pipeFD);

    testProcessPID = fork();
    if (testProcessPID < 0) {
        perror("fork");
        exit(errno);
    }
    if (testProcessPID == 0) { // child process
        string pipeToArgStr = "--pipe_to=" + pipeName;
        string maxParallelTestsArgStr = "--max_parallel_tests=" + to_string(maxParallelTests);
        char* cmd = (char*)testPath.c_str();
        char* pipeToArg = (char*)pipeToArgStr.c_str();
        char* maxParallelTestsArg = (char*)maxParallelTestsArgStr.c_str();
        char *argv[] = {cmd, "--quiet", "--boxed", pipeToArg, maxParallelTestsArg, nullptr};
        execve(cmd, argv, nullptr);
        perror("execve");
        exit(errno);
    }
}

void TestExecutionCycle::step() {
    if (finished) {
        return;
    }
    int ret = waitpid(testProcessPID, nullptr, WNOHANG);
    if (ret < 0) {
        perror("waitpid");
        exit(errno);
    }
    processMessages(false);
    if (ret != 0) { // process finished
        processMessages(true);
    }
}

bool TestExecutionCycle::isDone() const {
    return finished;
}

void TestExecutionCycle::processMessages(bool block) {
    while (!finished) {
        Message message = pipeWithTestProcess->getNextMessage();
        if (message.getPayload() != nullptr) {
            processMessage(message);
        } else if (!block) {
            break;
        }
    }
}

void TestExecutionCycle::processMessage(const Message& message) {
    MessageReader reader(message);
    int type;
    reader << type;
    // TODO: Make an enum in a shared space with the kktest library.
    if (type == 0) { // group
        GroupInfo groupInfo;
        reader << groupInfo.parentGroupIndex
               << groupInfo.index
               << groupInfo.line
               << groupInfo.file
               << groupInfo.description;
        info.groups[groupInfo.index] = groupInfo;
    } else if (type == 1) { // test
        TestInfo testInfo;
        reader << testInfo.groupIndex
               << testInfo.index
               << testInfo.line
               << testInfo.file
               << testInfo.optional
               << testInfo.description
               << testInfo.passed
               << testInfo.failureMessage;
        info.tests.push_back(testInfo);
        onTestCallback(info);
    } else if (type == 2) {
        int removeStat = remove(pipeName.c_str());
        if (removeStat < 0) {
            perror("remove pipe");
            exit(errno);
        }
        close(pipeFD);
        finished = true;
    }
}

}
