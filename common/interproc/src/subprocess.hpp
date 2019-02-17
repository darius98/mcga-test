#ifndef KKTEST_COMMON_INTERPROC_SRC_SUBPROCESS_HPP_
#define KKTEST_COMMON_INTERPROC_SRC_SUBPROCESS_HPP_

#include <functional>
#include <vector>

#include "common/interproc/src/message.hpp"
#include "common/interproc/src/pipe.hpp"

namespace kktest {
namespace interproc {

class Subprocess {
 public:
    enum FinishStatus { ZERO_EXIT, NON_ZERO_EXIT, SIGNALED, UNKNOWN };

    enum KillResult { KILLED, ALREADY_DEAD };

    static Subprocess* fork(const std::function<void()>& func);

    static Subprocess* open(char* executable, char* argv[]);

    static Subprocess* open(const std::string& executable,
                            const std::vector<std::string>& argv);

    virtual ~Subprocess() = default;

    virtual bool isFinished() = 0;

    virtual KillResult kill() = 0;

    virtual bool isExited() = 0;

    virtual int getReturnCode() = 0;

    virtual bool isSignaled() = 0;

    virtual int getSignal() = 0;

    virtual void wait() = 0;

    FinishStatus getFinishStatus();
};

}
}

#endif
