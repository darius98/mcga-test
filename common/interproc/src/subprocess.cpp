#include "common/interproc/src/subprocess.hpp"
#include "common/utils/src/string.hpp"

using namespace std;
using namespace kktest::utils;

namespace kktest {
namespace interproc {

Subprocess* Subprocess::open(const string& executable,
                             const vector<string>& argv) {
    char* executableCStr = copyAsCString(executable);
    auto cArgv = static_cast<char**>(malloc(sizeof(char*) * (argv.size() + 1)));
    for (size_t i = 0; i < argv.size(); ++ i) {
        cArgv[i] = copyAsCString(argv[i]);
    }
    cArgv[argv.size()] = nullptr;
    return open(executableCStr, cArgv);
}

Subprocess::FinishStatus Subprocess::getFinishStatus() {
    if (isSignaled()) {
        return SIGNALED;
    }
    if (!isExited()) {
        return UNKNOWN;
    }
    return getReturnCode() == 0 ? ZERO_EXIT : NON_ZERO_EXIT;
}

WorkerSubprocess::WorkerSubprocess(Subprocess* _subprocess,
                                   PipeReader* _pipeReader,
                                   PipeReader* _stdoutReader):
        subprocess(_subprocess),
        pipeReader(_pipeReader),
        stdoutReader(_stdoutReader) {}

WorkerSubprocess::WorkerSubprocess(WorkerSubprocess&& other) noexcept:
        subprocess(other.subprocess), pipeReader(other.pipeReader) {
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
    for (uint8_t byte : newBytes) {
        output += static_cast<char>(byte);
    }
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

WorkerSubprocess WorkerSubprocess::open(Work work) {
    auto pipe = createAnonymousPipe();
    auto stdoutPipe = createAnonymousPipe();
    auto worker = Subprocess::fork([&stdoutPipe, &pipe, &work]() {
        delete pipe.first;
        delete stdoutPipe.first;
        redirectStdoutToPipe(stdoutPipe.second);
        work(pipe.second);
        delete pipe.second;
        delete stdoutPipe.second;
    });
    delete pipe.second;
    delete stdoutPipe.second;
    return WorkerSubprocess(worker, pipe.first, stdoutPipe.first);
}

}
}
