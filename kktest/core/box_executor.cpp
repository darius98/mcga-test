#include "kktest/core/box_executor.hpp"

#include <thread>

#include <mcga/proc.hpp>

#include "kktest/core/time_tick.hpp"

using namespace mcga::proc;
using namespace std;
using std::this_thread::sleep_for;

namespace mcga::test {

enum PipeMessageType {
    WARNING,
    DONE,
};

RunningTest::RunningTest(Test test, BoxExecutor* executor)
        : test(move(test)), executor(executor) {
}

void RunningTest::startExecution() {
    auto timeLimit = TimeTicksToNanoseconds(test.getTimeTicksLimit()) + 1s;
    currentExecution = make_unique<WorkerSubprocess>(
      timeLimit, [this](PipeWriter* pipe) { executeBoxed(pipe); });
}

void RunningTest::executeBoxed(PipeWriter* pipe) const {
    executor->setCurrentTestingSubprocessPipe(pipe);
    ExecutedTest::Info info = executor->run(test);
    pipe->sendMessage(DONE, info.timeTicks, info.passed, info.failure);
}

bool RunningTest::finishedCurrentExecution() {
    Message message;
    bool passed = false;
    string error;
    switch (currentExecution->getFinishStatus()) {
        case WorkerSubprocess::NO_EXIT: {
            return false;
        }
        case WorkerSubprocess::ZERO_EXIT: {
            while (true) {
                message = currentExecution->getNextMessage(32);
                if (message.isInvalid()) {
                    error = "Unexpected 0-code exit.";
                    break;
                }
                auto messageType = message.read<PipeMessageType>();
                if (messageType == PipeMessageType::DONE) {
                    passed = true;
                    break;
                }
                // It's a warning
                executor->onWarning(Warning(message.read<string>(),
                                            test.getGroup()->getId(),
                                            test.getId()));
            }
            break;
        }
        case WorkerSubprocess::NON_ZERO_EXIT: {
            error = "Test exited with code "
              + to_string(currentExecution->getReturnCode());
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

BoxExecutor::BoxExecutor(size_t numBoxes): numBoxes(numBoxes) {
}

void BoxExecutor::execute(Test test) {
    ensureEmptyBoxes(1);
    auto runningTest = RunningTest(move(test), this);
    runningTest.startExecution();
    activeBoxes.push_back(move(runningTest));
}

void BoxExecutor::finalize() {
    ensureEmptyBoxes(numBoxes);
}

Executor::Type BoxExecutor::getType() const {
    return BOXED;
}

void BoxExecutor::emitWarning(const string& message) {
    currentTestingSubprocessPipe->sendMessage(WARNING, message);
}

void BoxExecutor::setCurrentTestingSubprocessPipe(PipeWriter* pipe) {
    currentTestingSubprocessPipe = pipe;
}

void BoxExecutor::ensureEmptyBoxes(size_t requiredEmpty) {
    size_t maxActive = numBoxes - requiredEmpty;
    while (activeBoxes.size() > maxActive) {
        bool progress = false;
        for (auto it = activeBoxes.begin(); it != activeBoxes.end();) {
            if (it->finishedCurrentExecution()) {
                if (it->finishedAllExecutions()) {
                    onTestFinished(move(*it).toExecutedTest());
                    it = activeBoxes.erase(it);
                } else {
                    it->startExecution();
                    ++it;
                }
                progress = true;
            } else {
                ++it;
            }
        }
        if (!progress) {
            sleep_for(5ms);
        }
    }
}

}  // namespace mcga::test
