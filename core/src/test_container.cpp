#include "core/src/test_container.hpp"

#include "core/src/test.hpp"

using namespace kktest::interproc;
using namespace std;

namespace kktest {

TestContainer::TestContainer(double timeLimitMs,
                             WorkerSubprocess::Work run,
                             Callback _callback):
        testProcessStopwatch(timeLimitMs),
        testWorker(WorkerSubprocess::open(run)),
        callback(move(_callback)) {}

bool TestContainer::poll() {
    auto finishWithError = [this](const string& errorMessage) {
        callback(TestExecutionInfo::toErrorMessage(errorMessage));
    };

    if (!testWorker.isFinished()) {
        if (!testProcessStopwatch.isElapsed()) {
            return false;
        }
        auto killStatus = testWorker.kill();
        if (killStatus == Subprocess::ALREADY_DEAD) {
            // The child might have finished during a context switch.
            // In this case, return false so we can retry waiting it later.
            return false;
        }
        finishWithError("Execution timed out.");
        return true;
    }
    switch (testWorker.getFinishStatus()) {
        case Subprocess::FinishStatus::UNKNOWN:
            finishWithError("Unknown error occurred.");
            break;
        case Subprocess::FinishStatus::SIGNALED:
            finishWithError("Killed by signal "
                            + to_string(testWorker.getSignal()));
            break;
        case Subprocess::FinishStatus::NON_ZERO_EXIT:
            finishWithError("Exit code "
                            + to_string(testWorker.getReturnCode()) + ".");
            break;
        case Subprocess::FinishStatus::ZERO_EXIT: {
            Message message = testWorker.getNextMessage();
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

}
