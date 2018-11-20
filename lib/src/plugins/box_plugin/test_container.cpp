#include <unistd.h>
#include <sys/wait.h>

#include <cstring>

#include <messaging>
#include <strutil>

#include <core/driver.hpp>
#include "test_container.hpp"

using namespace messaging;
using namespace strutil;
using namespace std;
using namespace std::chrono;


namespace kktest {

TestContainer::TestContainer(Test *_test, double _testProcessTimeLimitMs, Executable run):
        test(_test), testProcessTimeLimitMs(_testProcessTimeLimitMs) {
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
        TestingDriver::setHooksEnabled(false);
        run();
        OutputPipe(fd[1]).pipe(Message::build(test->isPassed(),
                                              test->getExecutionTimeTicks(),
                                              test->getFailureMessage()
        ));
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
        return finish(-1.0, false, "Non-zero exit code: " + to_string(WEXITSTATUS(wStatus)));
    }
    if (WIFSIGNALED(wStatus)) {
        return finish(-1.0, false, "Killed by signal " + to_string(WTERMSIG(wStatus)));
    }
    if (!WIFSIGNALED(wStatus) && !WIFEXITED(wStatus)) {
        return finish(-1.0, false, "Unknown error occurred.");
    }
    Message message = InputPipe(testProcessPipeFD).getNextMessage();
    if (message.getPayload() == nullptr) {
        return finish(-1.0, false, "Test unexpectedly exited with code 0");
    }
    MessageReader reader(message);
    bool isPassed;
    double ticks;
    string failureMessage;
    reader << isPassed << ticks << failureMessage;
    return finish(ticks, isPassed, unescapeCharacters(failureMessage));
}

Test* TestContainer::getTest() const {
    return test;
}

double TestContainer::getTicks() const {
    return ticks;
}

bool TestContainer::isPassed() const {
    return passed;
}

std::string TestContainer::getFailureMessage() const {
    return failureMessage;
}

bool TestContainer::finish(double _ticks, bool _passed, const string& _failureMessage) {
    close(testProcessPipeFD);
    ticks = _ticks;
    passed = _passed;
    failureMessage = _failureMessage;
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
    return finish(-1.0, false, "Execution timed out.");
}

}
