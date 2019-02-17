#ifndef KKTEST_COMMON_INTERPROC_SRC_TIMED_WORKER_SUBPROCESS_HPP_
#define KKTEST_COMMON_INTERPROC_SRC_TIMED_WORKER_SUBPROCESS_HPP_

#include "common/interproc/src/subprocess.hpp"
#include "common/utils/src/time.hpp"

namespace kktest {
namespace interproc {

// TODO(darius98): Merge this logic in the WorkerSubprocess class!
class TimedWorkerSubprocess {
 public:
    enum ResultType: std::uint8_t {
        ERROR = 0,
        SUCCESS = 1
    };

    typedef std::function<void(Message&)> Callback;

    TimedWorkerSubprocess(double timeLimitMs,
                          WorkerSubprocess::Work run,
                          Callback _callback);

    bool poll();

 private:
    bool finishWithError(const std::string& errorMessage);

    utils::ProcessStopwatch stopwatch;
    WorkerSubprocess worker;
    Callback callback;
};

}
}

#endif
