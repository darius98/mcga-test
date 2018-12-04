#include <kktest_common/interproc_impl/subprocess.hpp>

using std::function;
using std::pair;

namespace kktest {
namespace interproc {

WorkerSubprocess::WorkerSubprocess(SubprocessHandler* _subprocessHandler, PipeReader* _pipeReader):
        subprocessHandler(_subprocessHandler), pipeReader(_pipeReader) {}

WorkerSubprocess::WorkerSubprocess(WorkerSubprocess&& other) noexcept:
        subprocessHandler(other.subprocessHandler), pipeReader(other.pipeReader) {
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

}  // namespace interproc
}  // namespace kktest
