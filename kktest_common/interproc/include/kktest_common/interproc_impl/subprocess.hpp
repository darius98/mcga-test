#ifndef KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_SUBPROCESS_HPP_
#define KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_SUBPROCESS_HPP_

#include <functional>

namespace kktest {
namespace interproc {


class SubprocessHandler {
 public:
    enum KillResult { KILLED, ALREADY_DEAD };

    virtual ~SubprocessHandler() = default;

    virtual bool isFinished() = 0;

    virtual KillResult kill() = 0;

    virtual bool isExited() = 0;

    virtual int getReturnCode() = 0;

    virtual bool isSignaled() = 0;

    virtual int getSignal() = 0;
};

SubprocessHandler* forkAndRunInSubprocess(const std::function<void()>& func);

SubprocessHandler* openSubprocess(char* executable, char* argv[]);

}  // namespace interproc
}  // namespace kktest

#endif  // KKTEST_COMMON_INTERPROC_INCLUDE_KKTEST_COMMON_INTERPROC_IMPL_SUBPROCESS_HPP_
