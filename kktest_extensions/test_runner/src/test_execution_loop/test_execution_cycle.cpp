#include <unistd.h>
#include <sys/wait.h>

#include <cstdlib>

#include <stdexcept>
#include <iostream>

#include <kktest_common/strutil.hpp>
#include <kktest_ext/feedback.hpp>
#include "test_execution_cycle.hpp"

using kktest::interproc::Message;
using kktest::interproc::MessageReader;
using kktest::interproc::PipeReader;
using kktest::interproc::createNamedPipe;
using kktest::interproc::openNamedPipeForReading;
using kktest::strutil::copyAsCString;
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
    for (int i = 0; i < 10; ++i) {
        pipeName += static_cast<char>(rand() % 26 + 97);
    }
    createNamedPipe(pipeName.c_str());
    pipeWithTestProcess = openNamedPipeForReading(pipeName.c_str());

    testProcessPID = fork();
    if (testProcessPID < 0) {
        perror("fork");
        exit(errno);
    }
    if (testProcessPID == 0) {  // child process
        char* cmd = copyAsCString(testPath.c_str());
        char* quietArg = copyAsCString("--quiet");
        char* boxedArg = copyAsCString("--boxed");
        char* pipeToArg = copyAsCString(("--pipe_to=" + pipeName).c_str());
        char* maxParallelTestsArg = copyAsCString(
            ("--max_parallel_tests=" + to_string(maxParallelTests)).c_str());
        char* argv[] = {cmd, quietArg, boxedArg, pipeToArg, maxParallelTestsArg, nullptr};
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
    if (ret != 0) {  // process finished
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
    switch (type) {
        case PipeMessageType::GROUP: {
            GroupInfo groupInfo;
            reader << groupInfo.parentGroupIndex
                   << groupInfo.index
                   << groupInfo.description;
            info.groupsReceived.push_back(groupInfo);
            info.lastReceived = KKTestCaseInfo::GROUP;
            break;
        }
        case PipeMessageType::TEST: {
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
        case PipeMessageType::DONE: {
            int removeStat = remove(pipeName.c_str());
            if (removeStat < 0) {
                perror("remove pipe");
                exit(errno);
            }
            info.finished = true;
            info.lastReceived = KKTestCaseInfo::FINISH;
            delete pipeWithTestProcess;
            break;
        }
        case PipeMessageType::ERROR: {
            int removeStat = remove(pipeName.c_str());
            if (removeStat < 0) {
                perror("remove pipe");
                exit(errno);
            }
            info.finished = true;
            info.lastReceived = KKTestCaseInfo::FINISH_WITH_ERROR;
            reader << info.errorMessage;
            delete pipeWithTestProcess;
            break;
        }
        default: {
            cout << "Unknown message received on pipe, type " << type << ". Exiting.\n";
            exit(1);
        }
    }
    onInfoCallback(info);
}

}  // namespace test_runner
}  // namespace kktest
