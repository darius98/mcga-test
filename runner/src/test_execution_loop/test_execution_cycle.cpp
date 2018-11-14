#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <cstdlib>

#include <iostream>
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
    Message message = pipeWithTestProcess->getNextMessage();
    while (message.getPayload() != nullptr) {
        processMessage(message);
        message = pipeWithTestProcess->getNextMessage();
    }
    if (ret != 0) { // process finished
        int removeStat = remove(pipeName.c_str());
        close(pipeFD);
        finished = true;
    }
}

bool TestExecutionCycle::isDone() const {
    return finished;
}

void TestExecutionCycle::processMessage(const Message& message) {
    MessageReader reader(message);
    auto type = reader.read<int>();
    // TODO: Make an enum in a shared space with the kktest library.
    if (type == 0) { // group
        GroupInfo groupInfo;
        groupInfo.parentGroupIndex = reader.read<int>();
        groupInfo.index = reader.read<int>();
        groupInfo.line = reader.read<int>();
        groupInfo.file = reader.read<string>();
        groupInfo.description = reader.read<string>();
        info.groups[groupInfo.index] = groupInfo;
    } else if (type == 1) { // test
        TestInfo testInfo;
        testInfo.groupIndex = reader.read<int>();
        testInfo.index = reader.read<int>();
        testInfo.line = reader.read<int>();
        testInfo.file = reader.read<string>();
        testInfo.optional = reader.read<bool>();
        testInfo.description = reader.read<string>();
        testInfo.passed = reader.read<bool>();
        testInfo.failureMessage = reader.read<string>();
        info.tests.push_back(testInfo);
        onTestCallback(info);
    }
}

}
