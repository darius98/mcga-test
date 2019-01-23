#include <kktest_common/interproc_impl/subprocess.hpp>

using namespace std;

namespace kktest {
namespace interproc {

Subprocess* Subprocess::open(const String& executable,
                             const vector<String>& argv) {
    char* executableCStr = executable.copyAsCStr();
    char** cArgv = static_cast<char**>(
            malloc(sizeof(char*) * (argv.size() + 1)));
    for (size_t i = 0; i < argv.size(); ++ i) {
        cArgv[i] = argv[i].copyAsCStr();
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
                                   PipeReader* _pipeReader):
        subprocess(_subprocess), pipeReader(_pipeReader) {}

WorkerSubprocess::WorkerSubprocess(WorkerSubprocess&& other) noexcept:
        subprocess(other.subprocess), pipeReader(other.pipeReader) {
    other.subprocess = nullptr;
    other.pipeReader = nullptr;
}

WorkerSubprocess::~WorkerSubprocess() {
    delete subprocess;
    delete pipeReader;
}

Message WorkerSubprocess::getNextMessage() {
    return pipeReader->getNextMessage();
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

WorkerSubprocess WorkerSubprocess::open(Work work) {
    auto pipe = createAnonymousPipe();
    auto worker = Subprocess::fork([&pipe, &work]() {
        delete pipe.first;
        work(pipe.second);
        delete pipe.second;
    });
    delete pipe.second;
    return WorkerSubprocess(worker, pipe.first);
}

}
}
