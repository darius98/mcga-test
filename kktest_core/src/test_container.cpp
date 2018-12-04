#include "test_container.hpp"

using kktest::interproc::forkAndRunInSubprocess;
using kktest::interproc::Message;
using kktest::interproc::MessageReader;
using kktest::interproc::PipeWriter;
using kktest::interproc::SubprocessHandler;
using kktest::interproc::createAnonymousPipe;
using std::move;
using std::to_string;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace kktest {

TestContainer::TestContainer(double _timeLimitMs, SubprocessFunc run, Callback _callback):
        timeLimitMs(_timeLimitMs), callback(move(_callback)) {
    auto anonPipe = createAnonymousPipe();
    testProcess = forkAndRunInSubprocess([&anonPipe, &run]() {
        delete anonPipe.first;
        run(anonPipe.second);
        delete anonPipe.second;
    });
    delete anonPipe.second;
    pipe = anonPipe.first;
    startTime = high_resolution_clock::now();
}

bool TestContainer::poll() {
    if (!testProcess->isFinished()) {
        auto now = high_resolution_clock::now();
        double elapsedTimeMs = duration_cast<milliseconds>(now - startTime).count();
        if (elapsedTimeMs > timeLimitMs) {
            return killTestProcess();
        }
        return false;
    }
    if (testProcess->isExited() && testProcess->getReturnCode() != 0) {
        return finish("Non-zero exit code: " + to_string(testProcess->getReturnCode()));
    }
    if (testProcess->isSignaled()) {
        return finish("Killed by signal " + to_string(testProcess->getSignal()));
    }
    if (!testProcess->isSignaled() && !testProcess->isExited()) {
        return finish("Unknown error occurred.");
    }
    Message message = pipe->getNextMessage();
    if (message.getPayload() == nullptr) {
        return finish("Test unexpectedly exited with code 0");
    }
    return finish(message);
}

bool TestContainer::finish(const String& failureMessage) {
    return finish(Message::build(false, -1.0, failureMessage));
}

bool TestContainer::finish(const Message& message) {
    delete pipe;
    delete testProcess;
    callback(message);
    return true;
}

bool TestContainer::killTestProcess() {
    SubprocessHandler::KillResult killStatus = testProcess->kill();
    if (killStatus == SubprocessHandler::ALREADY_DEAD) {
        // The child might have finished during a context switch.
        // In this case, return false so we can retry waiting it later.
        return false;
    }
    return finish("Execution timed out.");
}

}  // namespace kktest
