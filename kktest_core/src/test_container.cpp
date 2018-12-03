#include <cstring>

#include <utility>

#include <kktest_common/strutil.hpp>
#include "test_container.hpp"

using kktest::interproc::forkAndRunInSubprocess;
using kktest::interproc::Message;
using kktest::interproc::MessageReader;
using kktest::interproc::PipeReader;
using kktest::interproc::PipeWriter;
using kktest::interproc::SubprocessHandler;
using kktest::interproc::createAnonymousPipe;
using kktest::strutil::unescapeCharacters;
using std::pair;
using std::to_string;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;
using std::chrono::milliseconds;

namespace kktest {

TestContainer::TestContainer(Test *_test, double _testProcessTimeLimitMs, Executable run):
        test(_test), testProcessTimeLimitMs(_testProcessTimeLimitMs) {
    pair<PipeReader*, PipeWriter*> pipe = createAnonymousPipe();
    testProcessHandler = forkAndRunInSubprocess([&pipe, &run, this]() {
        delete pipe.first;
        run();
        pipe.second->sendMessage(Message::build(test->isPassed(),
                                                test->getExecutionTimeTicks(),
                                                test->getFailureMessage()));
        delete pipe.second;
    });
    delete pipe.second;
    testProcessPipe = pipe.first;
    testProcessStartTime = high_resolution_clock::now();
}

bool TestContainer::isTestFinished() {
    if (!testProcessHandler->isFinished()) {
        auto now = high_resolution_clock::now();
        double elapsedTimeMs = duration_cast<milliseconds>(now - testProcessStartTime).count();
        if (elapsedTimeMs > testProcessTimeLimitMs) {
            return killTestProcess();
        }
        return false;
    }
    if (testProcessHandler->isExited() && testProcessHandler->getReturnCode() != 0) {
        return finish(
                -1.0,
                false,
                "Non-zero exit code: " + to_string(testProcessHandler->getReturnCode()));
    }
    if (testProcessHandler->isSignaled()) {
        return finish(
                -1.0,
                false,
                "Killed by signal " + to_string(testProcessHandler->getSignal()));
    }
    if (!testProcessHandler->isSignaled() && !testProcessHandler->isExited()) {
        return finish(-1.0, false, "Unknown error occurred.");
    }
    Message message = testProcessPipe->getNextMessage();
    if (message.getPayload() == nullptr) {
        return finish(-1.0, false, "Test unexpectedly exited with code 0");
    }
    MessageReader reader(message);
    bool isPassed;
    double ticks;
    String failureMessage;
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

String TestContainer::getFailureMessage() const {
    return failureMessage;
}

bool TestContainer::finish(double _ticks, bool _passed, const String& _failureMessage) {
    ticks = _ticks;
    passed = _passed;
    failureMessage = _failureMessage;
    delete testProcessPipe;
    delete testProcessHandler;
    return true;
}

bool TestContainer::killTestProcess() {
    SubprocessHandler::KillResult killStatus = testProcessHandler->kill();
    if (killStatus == SubprocessHandler::ALREADY_DEAD) {
        // The child might have finished during a context switch.
        // In this case, return false so we can retry waiting it later.
        return false;
    }
    return finish(-1.0, false, "Execution timed out.");
}

}  // namespace kktest
