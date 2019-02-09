#ifndef KKTEST_COMMON_INTERPROC_IMPL_SUBPROCESS_HPP_
#define KKTEST_COMMON_INTERPROC_IMPL_SUBPROCESS_HPP_

#include <functional>
#include <vector>

#include <kktest_common/interproc_impl/message.hpp>
#include <kktest_common/interproc_impl/pipe.hpp>

namespace kktest {
namespace interproc {

class Subprocess {
 public:
    enum FinishStatus { ZERO_EXIT, NON_ZERO_EXIT, SIGNALED, UNKNOWN };

    enum KillResult { KILLED, ALREADY_DEAD };

    static Subprocess* fork(const std::function<void()>& func);

    static Subprocess* open(char* executable, char* argv[]);

    static Subprocess* open(const String& executable,
                            const std::vector<String>& argv);

    virtual ~Subprocess() = default;

    virtual bool isFinished() = 0;

    virtual KillResult kill() = 0;

    virtual bool isExited() = 0;

    virtual int getReturnCode() = 0;

    virtual bool isSignaled() = 0;

    virtual int getSignal() = 0;

    FinishStatus getFinishStatus();
};

class WorkerSubprocess: public Subprocess {
 public:
    typedef const std::function<void(PipeWriter*)>& Work;

    static WorkerSubprocess open(Work work);

    WorkerSubprocess(WorkerSubprocess&& other) noexcept;

    WorkerSubprocess(const WorkerSubprocess& other) = delete;

    ~WorkerSubprocess() override;

    Message getNextMessage(int maxConsecutiveFailedReadAttempts = -1);

    bool isFinished() override;

    KillResult kill() override;

    bool isExited() override;

    int getReturnCode() override;

    bool isSignaled() override;

    int getSignal() override;

 private:
    WorkerSubprocess(Subprocess* _subprocess, PipeReader* _pipeReader);

    Subprocess* subprocess;
    PipeReader* pipeReader;
};

}
}

#endif
