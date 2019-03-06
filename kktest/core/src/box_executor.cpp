#include <memory>

#include "kktest/core/src/box_executor.hpp"

#include "common/interproc/src/pipe.hpp"
#include "common/utils/src/time.hpp"
#include "kktest/core/src/errors.hpp"

using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;

namespace kktest {

enum MessageStatus: uint8_t {
    SUCCESS = 0,
    CONFIGURATION_ERROR = 1
};

RunningTest::RunningTest(Test test): test(move(test)) {}

void RunningTest::startExecution(Executor* executor) {
    double timeLimitMs = test.getTimeTicksLimit()
                             * Executor::GetTimeTickLengthMs()
                         + 100.0;
    currentExecution = make_unique<WorkerSubprocess>(
        Duration::FromMs(timeLimitMs),
        bind(&RunningTest::executeBoxed, this, executor, placeholders::_1));
}

void RunningTest::executeBoxed(Executor *executor, PipeWriter *pipe) const {
    try {
        ExecutedTest::Info info = executor->run(test);
        pipe->sendMessage(SUCCESS, info.timeTicks, info.passed, info.failure);
    } catch(const ConfigurationError& error) {
        pipe->sendMessage(CONFIGURATION_ERROR, string(error.what()));
    }
}

bool RunningTest::finishedCurrentExecution() {
    bool passed = false;
    Message message;
    string error;
    switch (currentExecution->getFinishStatus()) {
        case WorkerSubprocess::NO_EXIT: {
            return false;
        }
        case WorkerSubprocess::ZERO_EXIT: {
            message = currentExecution->getNextMessage(32);
            if (message.isInvalid()) {
                passed = false;
                error = "Unexpected 0-code exit.";
            } else {
                passed = true;
            }
            break;
        }
        case WorkerSubprocess::NON_ZERO_EXIT: {
            error = "Test exited with code "
                    + to_string(currentExecution->getReturnCode()) + ".";
            break;
        }
        case WorkerSubprocess::SIGNAL_EXIT: {
            error = "Test killed by signal "
                    + to_string(currentExecution->getSignal());
            break;
        }
        case WorkerSubprocess::TIMEOUT: {
            error = "Test execution timed out.";
            break;
        }
    }
    if (!passed) {
        executions.emplace_back(move(error));
        return true;
    }
    if (message.read<MessageStatus>() == CONFIGURATION_ERROR) {
        // Read the error message and throw it.
        throw ConfigurationError(message.read<string>());
    }
    ExecutedTest::Info info;
    message >> info.timeTicks >> info.passed >> info.failure;
    executions.push_back(move(info));
    return true;
}

bool RunningTest::finishedAllExecutions() const {
    return executions.size() == test.getNumAttempts();
}

ExecutedTest RunningTest::toExecutedTest() && {
    return ExecutedTest(move(test), move(executions));
}

BoxExecutor::BoxExecutor(OnTestFinished onTestFinished, size_t numBoxes):
        Executor(move(onTestFinished)), numBoxes(numBoxes) {}

void BoxExecutor::execute(Test test) {
    ensureEmptyBoxes(1);
    activeBoxes.emplace_back(move(test));
    activeBoxes.back().startExecution(this);
}

void BoxExecutor::finalize() {
    ensureEmptyBoxes(numBoxes);
}

void BoxExecutor::ensureEmptyBoxes(size_t requiredEmpty) {
    size_t maxActive = numBoxes - requiredEmpty;
    while (activeBoxes.size() > maxActive) {
        bool progress = false;
        for (auto it = activeBoxes.begin(); it != activeBoxes.end(); ) {
            if (it->finishedCurrentExecution()) {
                if (it->finishedAllExecutions()) {
                    onTestFinished(move(*it).toExecutedTest());
                    it = activeBoxes.erase(it);
                } else {
                    it->startExecution(this);
                    ++ it;
                }
                progress = true;
            } else {
                ++ it;
            }
        }
        if (!progress) {
            sleepForDuration(5_ms);
        }
    }
}

}
