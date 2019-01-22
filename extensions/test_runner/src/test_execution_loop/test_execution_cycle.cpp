#include <cstdlib>

#include <stdexcept>
#include <iostream>

#include <kktest_common/string.hpp>
#include <kktest_ext/feedback.hpp>
#include "test_execution_cycle.hpp"

using namespace kktest;
using namespace kktest::feedback;
using namespace kktest::interproc;
using namespace std;

namespace kktest {

namespace test_runner {

TestExecutionCycle::TestExecutionCycle(
            const String& _testPath,
            int _maxParallelTests,
            const function<void(const KKTestCaseInfo&)>& _onInfoCallback):
        testPath(_testPath),
        maxParallelTests(_maxParallelTests),
        onInfoCallback(_onInfoCallback) {
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
    createNamedPipe(pipeName);
    pipeWithTestProcess = openNamedPipeForReading(pipeName);

    char* cmd = String(testPath).copyAsCStr();
    char* argv[] = {
        cmd,
        String("--quiet").copyAsCStr(),
        String("--boxed").copyAsCStr(),
        String("--pipe-to=" + pipeName).copyAsCStr(),
        String("--max-parallel-tests=" + to_string(maxParallelTests)).copyAsCStr(),
        nullptr,
    };
    testProcess = Subprocess::open(cmd, argv);
}

void TestExecutionCycle::step() {
    if (info.finished) {
        return;
    }
    processMessages(false);
    if (testProcess->isFinished()) {
        processMessages(true);
    }
}

bool TestExecutionCycle::isDone() const {
    return info.finished;
}

void TestExecutionCycle::processMessages(bool block) {
    while (!info.finished) {
        Message message = pipeWithTestProcess->getNextMessage();
        if (!message.isInvalid()) {
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
            destroyNamedPipe(pipeName);
            info.finished = true;
            info.lastReceived = KKTestCaseInfo::FINISH;
            delete pipeWithTestProcess;
            break;
        }
        case PipeMessageType::ERROR: {
            destroyNamedPipe(pipeName);
            info.finished = true;
            info.lastReceived = KKTestCaseInfo::FINISH_WITH_ERROR;
            reader << info.errorMessage;
            delete pipeWithTestProcess;
            break;
        }
        default: {
            cout << "Unknown message received on pipe, type "
                 << type
                 << ". Exiting.\n";
            exit(1);
        }
    }
    onInfoCallback(info);
}

}
}
