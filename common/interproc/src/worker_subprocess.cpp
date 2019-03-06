#include "common/interproc/src/worker_subprocess.hpp"

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
    auto stdoutPipe = createAnonymousPipe();
    subprocess = Subprocess::Fork([&stdoutPipe, &pipe, &run]() {
        delete pipe.first;
        delete stdoutPipe.first;
        redirectStdoutToPipe(stdoutPipe.second);
        run(pipe.second);
        delete pipe.second;
        delete stdoutPipe.second;
    });
    pipeReader = pipe.first;
    stdoutReader = stdoutPipe.first;

    delete pipe.second;
    delete stdoutPipe.second;
}

WorkerSubprocess::WorkerSubprocess(WorkerSubprocess&& other) noexcept:
        output(move(other.output)),
        subprocess(other.subprocess),
        pipeReader(other.pipeReader),
        stdoutReader(other.stdoutReader),
        stopwatch(other.stopwatch) {
    other.subprocess = nullptr;
    other.pipeReader = nullptr;
    other.stdoutReader = nullptr;
}

WorkerSubprocess::~WorkerSubprocess() {
    delete subprocess;
    delete pipeReader;
    delete stdoutReader;
}

Message WorkerSubprocess::getNextMessage(int maxConsecutiveFailedReadAttempts) {
    updateOutput();
    return pipeReader->getNextMessage(maxConsecutiveFailedReadAttempts);
}

bool WorkerSubprocess::isFinished() {
    updateOutput();
    return subprocess->isFinished();
}

Subprocess::KillResult WorkerSubprocess::kill() {
    updateOutput();
    return subprocess->kill();
}

bool WorkerSubprocess::isExited() {
    updateOutput();
    return subprocess->isExited();
}

int WorkerSubprocess::getReturnCode() {
    updateOutput();
    return subprocess->getReturnCode();
}

bool WorkerSubprocess::isSignaled() {
    updateOutput();
    return subprocess->isSignaled();
}

int WorkerSubprocess::getSignal() {
    updateOutput();
    return subprocess->getSignal();
}

string WorkerSubprocess::getOutput() {
    updateOutput();
    return output;
}

Subprocess::FinishStatus WorkerSubprocess::getFinishStatus() {
    updateOutput();
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

void WorkerSubprocess::updateOutput() {
    auto newBytes = stdoutReader->getBytes();
    output += string(newBytes.begin(), newBytes.end());
}

}
