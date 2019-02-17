#include "common/interproc/src/worker_subprocess.hpp"

using namespace std;

namespace kktest {
namespace interproc {

WorkerSubprocess::UnexpectedSubprocessEnd::UnexpectedSubprocessEnd(
        const string& message): runtime_error(message) {}

WorkerSubprocess::WorkerSubprocess(double timeLimitMs, Work run):
        stopwatch(timeLimitMs) {
    auto pipe = createAnonymousPipe();
    auto stdoutPipe = createAnonymousPipe();
    subprocess = Subprocess::fork([&stdoutPipe, &pipe, &run]() {
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
    return pipeReader->getNextMessage(maxConsecutiveFailedReadAttempts);
}

bool WorkerSubprocess::isFinished() {
    auto newBytes = stdoutReader->getBytes();
    output += string(newBytes.begin(), newBytes.end());
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

string WorkerSubprocess::getOutput() {
    return output;
}

void WorkerSubprocess::wait() {
    subprocess->wait();
    auto newBytes = stdoutReader->getBytes();
    for (uint8_t byte : newBytes) {
        output += static_cast<char>(byte);
    }
}

bool WorkerSubprocess::poll() {
    if (!isFinished()) {
        if (!stopwatch.isElapsed()) {
            return false;
        }
        auto killStatus = kill();
        if (killStatus == Subprocess::ALREADY_DEAD) {
            // The child might have finished during a context switch.
            // In this case, return false so we can retry waiting it later.
            return false;
        }
        onUnexpectedExit("Execution timed out.");
    }
    switch (getFinishStatus()) {
        case Subprocess::FinishStatus::UNKNOWN:
            onUnexpectedExit("Unknown error occurred.");
        case Subprocess::FinishStatus::SIGNALED:
            onUnexpectedExit("Killed by signal " + to_string(getSignal()));
        case Subprocess::FinishStatus::NON_ZERO_EXIT:
            onUnexpectedExit("Exit code " + to_string(getReturnCode()) + ".");
        case Subprocess::FinishStatus::ZERO_EXIT:
            break;
        default:
            onUnexpectedExit("Unknown error occurred.");
    }
    return true;
}

void WorkerSubprocess::onUnexpectedExit(const string &errorMessage) {
    throw UnexpectedSubprocessEnd(errorMessage);
}

}
}
