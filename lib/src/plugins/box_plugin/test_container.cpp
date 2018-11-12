#include <unistd.h>
#include <sys/wait.h>

#include <cstring>

#include <utils/message.hpp>
#include <utils/pipe.hpp>
#include <utils/unescape_characters.hpp>
#include "test_container.hpp"

using namespace kktest::utils;
using namespace std;
using namespace std::chrono;


namespace kktest {

TestContainer::TestContainer(Test *_test,
                             double _testProcessTimeLimitMs,
                             Executable run,
                             CopyableExecutable _afterTestCallback):
        test(_test),
        testProcessTimeLimitMs(_testProcessTimeLimitMs),
        afterTestCallback(move(_afterTestCallback)) {
    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(errno);
    }
    testProcessPipeFD = fd[0];
    testProcessPID = fork();
    if (testProcessPID < 0) {
        perror("fork");
        exit(errno);
    }
    if (testProcessPID == 0) { // child
        close(testProcessPipeFD);
        run();
        OutputPipe(fd[1]).pipe(Message::build([this](BytesConsumer& consumer) {
            consumer << test->isPassed()
                     << test->getExecutionTimeTicks()
                     << test->getFailureMessage();
        }));
        exit(0);
    }
    close(fd[1]);
    testProcessStartTime = high_resolution_clock::now();
}

bool TestContainer::isTestFinished() {
    int wStatus;
    int ret = waitpid(testProcessPID, &wStatus, WNOHANG);
    if (ret < 0) {
        perror("waitpid");
        exit(errno);
    }
    if (ret == 0) {
        auto now = high_resolution_clock::now();
        double elapsedTimeMs = duration_cast<milliseconds>(now - testProcessStartTime).count();
        if (elapsedTimeMs > testProcessTimeLimitMs) {
            return killTestProcess();
        }
        return false;
    }
    if (WIFEXITED(wStatus) && WEXITSTATUS(wStatus) != 0) {
        test->setExecuted(-1.0);
        test->setFailure("Non-zero exit code: " + to_string(WEXITSTATUS(wStatus)));
        return finish();
    }
    if (WIFSIGNALED(wStatus)) {
        test->setExecuted(-1.0);
        test->setFailure("Killed by signal " + to_string(WTERMSIG(wStatus)));
        return finish();
    }
    if (!WIFSIGNALED(wStatus) && !WIFEXITED(wStatus)) {
        test->setExecuted(-1.0);
        test->setFailure("Unknown error occurred.");
        return finish();
    }
    Message message = InputPipe(testProcessPipeFD).getNextMessage();
    if (message.getPayload() == nullptr) {
        test->setExecuted(-1.0);
        test->setFailure("Test unexpectedly exited with code 0");
        return finish();
    }
    MessageReader reader(message);
    auto isPassed = reader.read<bool>();
    auto ticks = reader.read<double>();
    string failureMessage = reader.read<string>();
    test->setExecuted(ticks);
    if (!isPassed) {
        test->setFailure(unescapeCharacters(failureMessage));
    }
    return finish();
}

bool TestContainer::finish() {
    close(testProcessPipeFD);
    afterTestCallback();
    return true;
}

bool TestContainer::killTestProcess() {
    int killStatus = kill(testProcessPID, SIGKILL);
    if (killStatus < 0) {
        if (errno == ESRCH) {
            // The child might have finished during a context switch.
            // In this case, return false so we can retry waiting it later.
            return false;
        }
        perror("kill");
        exit(errno);
    }
    test->setExecuted(-1.0);
    return finish();
}

}
