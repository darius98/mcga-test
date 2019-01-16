#include <kktest_common/interproc_impl/subprocess.hpp>

using namespace std;

namespace kktest {
namespace interproc {

WorkerSubprocess::WorkerSubprocess(SubprocessHandler* _subprocessHandler,
                                   PipeReader* _pipeReader):
        subprocessHandler(_subprocessHandler), pipeReader(_pipeReader) {}

WorkerSubprocess::WorkerSubprocess(WorkerSubprocess&& other) noexcept:
        subprocessHandler(other.subprocessHandler),
        pipeReader(other.pipeReader) {
    other.subprocessHandler = nullptr;
    other.pipeReader = nullptr;
}

WorkerSubprocess::~WorkerSubprocess() {
    delete subprocessHandler;
    delete pipeReader;
}

SubprocessHandler* WorkerSubprocess::getSubprocessHandler() {
    return subprocessHandler;
}

PipeReader* WorkerSubprocess::getPipe() {
    return pipeReader;
}

SubprocessHandler::FinishStatus SubprocessHandler::getFinishStatus() {
    if (isSignaled()) {
        return SIGNALED;
    }
    if (!isExited()) {
        return UNKNOWN;
    }
    return getReturnCode() == 0 ? ZERO_EXIT : NON_ZERO_EXIT;
}

WorkerSubprocess forkAndRunWorkerSubprocess(SubprocessWork work) {
    auto pipe = createAnonymousPipe();
    auto worker = forkAndRunInSubprocess([&pipe, &work]() {
        delete pipe.first;
        work(pipe.second);
        delete pipe.second;
    });
    delete pipe.second;
    return WorkerSubprocess(worker, pipe.first);
}

}
}
