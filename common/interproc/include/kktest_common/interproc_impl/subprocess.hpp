#ifndef KKTEST_COMMON_INTERPROC_IMPL_SUBPROCESS_HPP_
#define KKTEST_COMMON_INTERPROC_IMPL_SUBPROCESS_HPP_

#include <functional>

#include <kktest_common/interproc_impl/pipe.hpp>

namespace kktest {
namespace interproc {

class SubprocessHandler {
 public:
    enum FinishStatus { ZERO_EXIT, NON_ZERO_EXIT, SIGNALED, UNKNOWN };

    enum KillResult { KILLED, ALREADY_DEAD };

    virtual ~SubprocessHandler() = default;

    virtual bool isFinished() = 0;

    virtual KillResult kill() = 0;

    virtual bool isExited() = 0;

    virtual int getReturnCode() = 0;

    virtual bool isSignaled() = 0;

    virtual int getSignal() = 0;

    FinishStatus getFinishStatus();
};

SubprocessHandler* forkAndRunInSubprocess(const std::function<void()>& func);

SubprocessHandler* openSubprocess(char* executable, char* argv[]);

typedef const std::function<void(PipeWriter*)>& SubprocessWork;

class WorkerSubprocess {
 public:
    WorkerSubprocess(SubprocessHandler* _subprocessHandler,
                     PipeReader* _pipeReader);
    WorkerSubprocess(WorkerSubprocess&& other) noexcept;

    WorkerSubprocess(const WorkerSubprocess& other) = delete;

    ~WorkerSubprocess();

    SubprocessHandler* getSubprocessHandler();

    PipeReader* getPipe();

 private:
    SubprocessHandler* subprocessHandler;
    PipeReader* pipeReader;
};

WorkerSubprocess forkAndRunWorkerSubprocess(SubprocessWork work);

}
}

#endif
