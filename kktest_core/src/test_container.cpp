#include "test_container.hpp"
#include "test.hpp"

using kktest::interproc::forkAndRunWorkerSubprocess;
using kktest::interproc::Message;
using kktest::interproc::SubprocessHandler;
using kktest::interproc::SubprocessWork;
using std::move;
using std::to_string;

namespace kktest {

TestContainer::TestContainer(double timeLimitMs,
                             SubprocessWork run,
                             Callback _callback):
        testProcessStopwatch(timeLimitMs),
        testWorker(forkAndRunWorkerSubprocess(run)),
        callback(move(_callback)) {}

bool TestContainer::poll() {
    auto finishWithError = [this](const String& errorMessage) {
        callback(TestExecutionInfo::toErrorMessage(errorMessage));
    };

    auto testProcess = testWorker.getSubprocessHandler();
    if (!testProcess->isFinished()) {
        if (!testProcessStopwatch.isElapsed()) {
            return false;
        }
        auto killStatus = testWorker.getSubprocessHandler()->kill();
        if (killStatus == SubprocessHandler::ALREADY_DEAD) {
            // The child might have finished during a context switch.
            // In this case, return false so we can retry waiting it later.
            return false;
        }
        finishWithError("Execution timed out.");
        return true;
    }
    switch (testProcess->getFinishStatus()) {
        case SubprocessHandler::FinishStatus::UNKNOWN:
            finishWithError("Unknown error occurred.");
            break;
        case SubprocessHandler::FinishStatus::SIGNALED:
            finishWithError("Killed by signal "
                            + to_string(testProcess->getSignal()));
            break;
        case SubprocessHandler::FinishStatus::NON_ZERO_EXIT:
            finishWithError("Exit code "
                            + to_string(testProcess->getReturnCode()) + ".");
            break;
        case SubprocessHandler::FinishStatus::ZERO_EXIT: {
            Message message = testWorker.getPipe()->getNextMessage();
            if (message.isInvalid()) {
                finishWithError("Test unexpectedly exited with code 0");
            }
            callback(message);
            break;
        }
        default:
            finishWithError("Unknown error occurred.");
            break;
    }
    return true;
}

}  // namespace kktest
