#pragma once

#include <chrono>

#include "common/interproc/subprocess.hpp"
#include "common/interproc/pipe.hpp"

namespace kktest::interproc {

class WorkerSubprocess: public Subprocess {
 public:
    typedef const std::function<void(PipeWriter*)>& Work;

    WorkerSubprocess(const std::chrono::nanoseconds& timeLimit, Work run);

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

    Message getNextMessage(int maxConsecutiveFailedReadAttempts = -1);

 private:
    Subprocess* subprocess;
    PipeReader* pipeReader;
    std::chrono::high_resolution_clock::time_point endTime;
};

}
