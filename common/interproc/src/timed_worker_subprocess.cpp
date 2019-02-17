#include "common/interproc/src/timed_worker_subprocess.hpp"

using namespace std;

namespace kktest {
namespace interproc {

TimedWorkerSubprocess::TimedWorkerSubprocess(double timeLimitMs,
                                             WorkerSubprocess::Work run,
                                             Callback _callback):
        stopwatch(timeLimitMs),
        worker(WorkerSubprocess::open(run)),
        callback(move(_callback)) {}

bool TimedWorkerSubprocess::poll() {
    if (!worker.isFinished()) {
        if (!stopwatch.isElapsed()) {
            return false;
        }
        auto killStatus = worker.kill();
        if (killStatus == Subprocess::ALREADY_DEAD) {
            // The child might have finished during a context switch.
            // In this case, return false so we can retry waiting it later.
            return false;
        }
        return finishWithError("Execution timed out.");
    }
    switch (worker.getFinishStatus()) {
        case Subprocess::FinishStatus::UNKNOWN:
            return finishWithError("Unknown error occurred.");
        case Subprocess::FinishStatus::SIGNALED:
            return finishWithError("Killed by signal "
                                   + to_string(worker.getSignal()));
        case Subprocess::FinishStatus::NON_ZERO_EXIT:
            return finishWithError("Exit code "
                                   + to_string(worker.getReturnCode()) + ".");
        case Subprocess::FinishStatus::ZERO_EXIT: {
            Message message = worker.getNextMessage();
            if (message.isInvalid()) {
                return finishWithError("Unexpected 0-code exit.");
            }
            auto successMessage = Message::build(SUCCESS, message);
            callback(successMessage);
            return true;
        }
        default:
            return finishWithError("Unknown error occurred.");
    }
}

bool TimedWorkerSubprocess::finishWithError(const string& errorMessage) {
    auto message = Message::build(ERROR, errorMessage);
    callback(message);
    return true;
}

}
}