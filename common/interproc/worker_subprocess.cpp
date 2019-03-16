#include "common/interproc/worker_subprocess.hpp"

using namespace kktest::utils;
using namespace std;

namespace kktest::interproc {

WorkerSubprocess::WorkerSubprocess(Duration timeLimit, Work run):
// TODO(darius98): I am not sure exactly what I want here.
//  The current implementation uses a real-time stopwatch to set the
//  subprocess's time limit. Is this the desired behaviour or is it
//  to take into account only the subprocess' CPU time?
        stopwatch(timeLimit) {
    auto pipe = createAnonymousPipe();
    subprocess = Subprocess::Fork([&pipe, &run]() {
        delete pipe.first;
        run(pipe.second);
        delete pipe.second;
    });
    pipeReader = pipe.first;

    delete pipe.second;
}

WorkerSubprocess::WorkerSubprocess(WorkerSubprocess&& other) noexcept:
        subprocess(other.subprocess),
        pipeReader(other.pipeReader),
        stopwatch(other.stopwatch) {
    other.subprocess = nullptr;
    other.pipeReader = nullptr;
}

WorkerSubprocess::~WorkerSubprocess() {
    delete subprocess;
    delete pipeReader;
}

Message WorkerSubprocess::getNextMessage(int maxConsecutiveFailedReadAttempts) {
    return pipeReader->getNextMessage(maxConsecutiveFailedReadAttempts);
}

bool WorkerSubprocess::isFinished() {
    return subprocess->isFinished();
}

Subprocess::KillResult WorkerSubprocess::kill() {
    return subprocess->kill();
}

bool WorkerSubprocess::isExited() {
    return subprocess->isExited();
}

int WorkerSubprocess::getReturnCode() {
    return subprocess->getReturnCode();
}

bool WorkerSubprocess::isSignaled() {
    return subprocess->isSignaled();
}

int WorkerSubprocess::getSignal() {
    return subprocess->getSignal();
}

Subprocess::FinishStatus WorkerSubprocess::getFinishStatus() {
    if (!isFinished()) {
        if (!stopwatch.isElapsed()) {
            return NO_EXIT;
        }
        auto killStatus = kill();
        if (killStatus == Subprocess::ALREADY_DEAD) {
            // The child might have finished during a context switch.
            // In this case, return false so we can retry waiting it later.
            return NO_EXIT;
        }
        return TIMEOUT;
    }
    return subprocess->getFinishStatus();
}

}
