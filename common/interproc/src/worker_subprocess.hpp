#ifndef KKTEST_COMMON_INTERPROC_SRC_WORKER_SUBPROCESS_HPP_
#define KKTEST_COMMON_INTERPROC_SRC_WORKER_SUBPROCESS_HPP_

#include "common/interproc/src/subprocess.hpp"
#include "common/interproc/src/pipe.hpp"
#include "common/utils/src/time.hpp"

namespace kktest {
namespace interproc {

class WorkerSubprocess: public Subprocess {
 public:
    typedef const std::function<void(PipeWriter*)>& Work;

    WorkerSubprocess(double timeLimitMs, Work run);

    WorkerSubprocess(WorkerSubprocess&& other) noexcept;

    WorkerSubprocess(const WorkerSubprocess& other) = delete;

    ~WorkerSubprocess() override;

    bool isFinished() override;

    KillResult kill() override;

    bool isExited() override;

    int getReturnCode() override;

    bool isSignaled() override;

    int getSignal() override;

    FinishStatus getFinishStatus() override;

    std::string getOutput();

    Message getNextMessage(int maxConsecutiveFailedReadAttempts = -1);

 private:
    void updateOutput();

    std::string output;
    Subprocess* subprocess;
    PipeReader* pipeReader;
    PipeReader* stdoutReader;
    utils::RealTimeStopwatch stopwatch;
};

}
}

#endif
