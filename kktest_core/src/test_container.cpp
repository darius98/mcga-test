#include "test_container.hpp"

using kktest::interproc::forkAndRunWorkerSubprocess;
using kktest::interproc::Message;
using kktest::interproc::SubprocessHandler;
using kktest::interproc::SubprocessWork;
using std::move;
using std::to_string;

namespace kktest {

TestContainer::TestContainer(double timeLimitMs, SubprocessWork run, Callback _callback):
        testProcessStopwatch(timeLimitMs),
        testWorker(forkAndRunWorkerSubprocess(run)),
        callback(move(_callback)) {}

bool TestContainer::poll() {
    auto testProcess = testWorker.getSubprocessHandler();
    if (!testProcess->isFinished()) {
        if (testProcessStopwatch.isElapsed()) {
            return killTestProcess();
        }
        return false;
    }
    if (testProcess->isSignaled()) {
        return finishWithError("Killed by signal " + to_string(testProcess->getSignal()));
    }
    if (!testProcess->isExited()) {
        return finishWithError("Unknown error occurred.");
    }
    if (testProcess->getReturnCode() != 0) {
        return finishWithError("Non-zero exit code: " + to_string(testProcess->getReturnCode()));
    }
    Message message = testWorker.getPipe()->getNextMessage();
    if (message.getPayload() == nullptr) {
        return finishWithError("Test unexpectedly exited with code 0");
    }
    callback(message);
    return true;
}

bool TestContainer::finishWithError(const String& failureMessage) {
    callback(Message::build(false, -1.0, failureMessage));
    return true;
}

bool TestContainer::killTestProcess() {
    SubprocessHandler::KillResult killStatus = testWorker.getSubprocessHandler()->kill();
    if (killStatus == SubprocessHandler::ALREADY_DEAD) {
        // The child might have finished during a context switch.
        // In this case, return false so we can retry waiting it later.
        return false;
    }
    return finishWithError("Execution timed out.");
}

}  // namespace kktest
