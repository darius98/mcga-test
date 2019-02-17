#ifndef KKTEST_COMMON_INTERPROC_SRC_WORKER_SUBPROCESS_HPP_
#define KKTEST_COMMON_INTERPROC_SRC_WORKER_SUBPROCESS_HPP_

#include "common/interproc/src/subprocess.hpp"
#include "common/utils/src/time.hpp"

namespace kktest {
namespace interproc {

class WorkerSubprocess: public Subprocess {
 public:
    typedef const std::function<void(PipeWriter*)>& Work;

    typedef std::function<void(Message&)> Callback;

    enum ResultType: std::uint8_t {
        ERROR = 0,
        SUCCESS = 1
    };

    WorkerSubprocess(double timeLimitMs, Work run, Callback _callback);

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

    void wait() override;

    std::string getOutput();

    // TODO(darius98): This method should really not call a callback,
    //            ...  consuming the next message.
    bool poll();

 private:
    bool finishWithError(const std::string& errorMessage);

    std::string output;
    Subprocess* subprocess;
    PipeReader* pipeReader;
    PipeReader* stdoutReader;
    utils::ProcessStopwatch stopwatch;

    // TODO(darius98): Investigate if this can be removed. If so, remove it.
    Callback callback;
};

}
}

#endif
