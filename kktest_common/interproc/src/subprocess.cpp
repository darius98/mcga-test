#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <cstdlib>

#include <kktest_common/interproc_impl/subprocess.hpp>

using std::function;

namespace kktest {
namespace interproc {


class LinuxSubprocessHandler: public SubprocessHandler {
 public:
    explicit LinuxSubprocessHandler(pid_t _pid): pid(_pid) {}

    bool isFinished() override {
        if (killed || finished) {
            return true;
        }
        int wStatus;
        int ret = waitpid(pid, &wStatus, WNOHANG);
        if (ret < 0) {
            perror("waitpid");
            exit(errno);
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
            perror("kill");
            exit(errno);
        }
        return KILLED;
    }

    bool isExited() override {
        return WIFEXITED(lastWaitStatus);
    }

    int getReturnCode() override {
        if (isExited()) {
            return WEXITSTATUS(lastWaitStatus);
        }
        return -1;
    }

    bool isSignaled() override {
        return WIFSIGNALED(lastWaitStatus);
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

SubprocessHandler* forkAndRunInSubprocess(const function<void()>& func) {
    pid_t forkPid = fork();
    if (forkPid < 0) {
        perror("fork");
        exit(errno);
    }
    if (forkPid == 0) {  // child process
        func();
        exit(0);
    }
    return new LinuxSubprocessHandler(forkPid);
}

SubprocessHandler* openSubprocess(char* executable, char* argv[]) {
    pid_t forkPid = fork();
    if (forkPid < 0) {
        perror("fork");
        exit(errno);
    }
    if (forkPid == 0) {  // child process
        execve(executable, argv, nullptr);
        perror("execve");
        exit(errno);
    }
    return new LinuxSubprocessHandler(forkPid);
}

}  // namespace interproc
}  // namespace kktest
