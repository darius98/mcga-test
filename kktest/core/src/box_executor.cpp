#include "kktest/core/src/box_executor.hpp"

#include "common/interproc/src/pipe.hpp"
#include "common/utils/src/time.hpp"
#include "kktest/core/src/errors.hpp"

using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;
using namespace std::placeholders;

namespace kktest {

RunningTest::RunningTest(Test test): test(move(test)) {}

void RunningTest::startExecution(Executor* executor) {
    auto timeLimit = timeTicksToNanoseconds(test.getTimeTicksLimit()) + 1s;
    currentExecution = make_unique<WorkerSubprocess>(
        timeLimit,
        bind(&RunningTest::executeBoxed, this, executor, _1));
}

void RunningTest::executeBoxed(Executor *executor, PipeWriter *pipe) const {
    ExecutedTest::Info info = executor->run(test);
    pipe->sendMessage(info.timeTicks, info.passed, info.failure);
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
            sleep(5ms);
        }
    }
}

}
