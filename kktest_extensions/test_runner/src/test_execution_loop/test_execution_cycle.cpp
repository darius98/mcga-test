#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <cstdlib>

#include <stdexcept>
#include <iostream>

#include <kktest_ext/feedback>
#include "test_execution_cycle.hpp"

using messaging::InputPipe;
using messaging::Message;
using messaging::MessageReader;
using std::cout;
using std::function;
using std::runtime_error;
using std::to_string;

namespace kktest {

using feedback::PipeMessageType;

namespace test_runner {

TestExecutionCycle::TestExecutionCycle(
            const String& _testPath,
            int _maxParallelTests,
            const function<void(const KKTestCaseInfo&)>& _onInfoCallback):
        started(false),
        testPath(_testPath),
        maxParallelTests(_maxParallelTests),
        onInfoCallback(_onInfoCallback),
        pipeWithTestProcess(nullptr),
        testProcessPID(0) {
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
        String quietArgStr = "--quiet";
        String boxedArgStr = "--boxed";
        String pipeToArgStr = "--pipe_to=" + pipeName;
        String maxParallelTestsArgStr = "--max_parallel_tests=" + to_string(maxParallelTests);
        auto cmd = (char*)testPath.c_str();
        auto quietArg = (char*)quietArgStr.c_str();
        auto boxedArg = (char*)boxedArgStr.c_str();
        auto pipeToArg = (char*)pipeToArgStr.c_str();
        auto maxParallelTestsArg = (char*)maxParallelTestsArgStr.c_str();
        char *const argv[] = {cmd, quietArg, boxedArg, pipeToArg, maxParallelTestsArg, nullptr};
        execve(cmd, argv, nullptr);
        perror("execve");
        exit(errno);
    }
}

void TestExecutionCycle::step() {
    if (info.finished) {
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
    return info.finished;
}

void TestExecutionCycle::processMessages(bool block) {
    while (!info.finished) {
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
    PipeMessageType type;
    reader << type;
    switch(type) {
        case PipeMessageType::GROUP: { // group
            GroupInfo groupInfo;
            reader << groupInfo.parentGroupIndex
                   << groupInfo.index
                   << groupInfo.description;
            info.groupsReceived.push_back(groupInfo);
            info.lastReceived = KKTestCaseInfo::GROUP;
            break;
        }
        case PipeMessageType::TEST: { // test
            TestInfo testInfo;
            reader << testInfo.groupIndex
                   << testInfo.index
                   << testInfo.optional
                   << testInfo.description
                   << testInfo.passed
                   << testInfo.failureMessage;
            info.testsReceived.push_back(testInfo);
            info.lastReceived = KKTestCaseInfo::TEST;
            break;
        }
        case PipeMessageType::DONE: { // done
            int removeStat = remove(pipeName.c_str());
            if (removeStat < 0) {
                perror("remove pipe");
                exit(errno);
            }
            close(pipeFD);
            info.finished = true;
            info.lastReceived = KKTestCaseInfo::FINISH;
            break;
        }
        case PipeMessageType::ERROR: { // error
            int removeStat = remove(pipeName.c_str());
            if (removeStat < 0) {
                perror("remove pipe");
                exit(errno);
            }
            close(pipeFD);
            info.finished = true;
            info.lastReceived = KKTestCaseInfo::FINISH_WITH_ERROR;
            reader << info.errorMessage;
            break;
        }
        default: { // idk
            cout << "Unknown message received on pipe, type " << type << ". Exiting.\n";
            exit(1);
        }

    }
    onInfoCallback(info);
}

}
}
