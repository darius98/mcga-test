#include "common/interproc/src/subprocess.hpp"

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>

#include "common/interproc/src/errors.hpp"

using namespace std;

namespace kktest {
namespace interproc {


class LinuxSubprocessHandler: public Subprocess {
 public:
    explicit LinuxSubprocessHandler(pid_t pid): pid(pid) {}

    bool isFinished() override {
        if (killed || finished) {
            return true;
        }
        int wStatus;
        int ret = waitpid(pid, &wStatus, WNOHANG);
        if (ret < 0) {
            throw InterprocError(strerror(errno));
        }
        if (ret == 0) {
            return false;
        }
        finished = true;
        lastWaitStatus = wStatus;
        return true;
    }

    KillResult kill() override {
        int killStatus = ::kill(pid, SIGKILL);
        if (killStatus < 0) {
            if (errno == ESRCH) {
                return ALREADY_DEAD;
            }
            throw InterprocError(strerror(errno));
        }
        return KILLED;
    }

    bool isExited() override {
        return finished && WIFEXITED(lastWaitStatus);
    }

    int getReturnCode() override {
        if (isExited()) {
            return WEXITSTATUS(lastWaitStatus);
        }
        return -1;
    }

    bool isSignaled() override {
        return finished && WIFSIGNALED(lastWaitStatus);
    }

    int getSignal() override {
        if (isSignaled()) {
            return WTERMSIG(lastWaitStatus);
        }
        return -1;
    }

 private:
    pid_t pid;
    bool killed = false;
    bool finished = false;
    int lastWaitStatus = 0;
};

Subprocess* Subprocess::fork(const function<void()>& func) {
    pid_t forkPid = ::fork();
    if (forkPid < 0) {
        throw InterprocError(strerror(errno));
    }
    if (forkPid == 0) {  // child process
        func();
        exit(0);
    }
    return new LinuxSubprocessHandler(forkPid);
}

}
}
