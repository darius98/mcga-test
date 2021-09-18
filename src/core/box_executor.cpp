#include "box_executor.hpp"

#include <algorithm>
#include <thread>

#include "time_tick.hpp"

using mcga::proc::Message;
using mcga::proc::PipeWriter;
using mcga::proc::Subprocess;
using mcga::proc::WorkerSubprocess;

namespace mcga::test {

enum PipeMessageType {
    WARNING,
    DONE,
};

BoxExecutor::BoxExecutor(HooksManager* hooks, size_t numBoxes)
        : Executor(hooks), numBoxes(numBoxes) {
}

void BoxExecutor::execute(Test test) {
    ensureEmptyBoxes(1);
    onTestExecutionStart(test);
    auto firstExecution = this->startExecution(test);
    activeBoxes.emplace_back(std::move(test), std::move(firstExecution));
}

void BoxExecutor::finalize() {
    ensureEmptyBoxes(numBoxes);
}

Executor::Type BoxExecutor::getType() const {
    return BOXED;
}

void BoxExecutor::emitWarning(const std::string& message,
                              std::size_t /*groupId*/) {
    if (isActive()) {
        currentTestingSubprocessPipe->sendMessage(WARNING, message);
    }
}

void BoxExecutor::executeBoxed(const Test& test,
                               std::unique_ptr<PipeWriter> pipe) {
    currentTestingSubprocessPipe = std::move(pipe);
    Test::ExecutionInfo info = run(test);
    currentTestingSubprocessPipe->sendMessage(
      DONE, info.timeTicks, info.passed, info.failure);
}

std::unique_ptr<WorkerSubprocess>
  BoxExecutor::startExecution(const Test& test) {
    auto timeLimit = TimeTicksToNanoseconds(test.getTimeTicksLimit())
      + std::chrono::seconds(1);
    return make_unique<WorkerSubprocess>(
      timeLimit, [this, &test](std::unique_ptr<PipeWriter> pipe) {
          executeBoxed(test, std::move(pipe));
      });
}

bool BoxExecutor::tryCloseBox(Box* box) {
    Test& test = box->first;
    WorkerSubprocess* process = box->second.get();

    Test::ExecutionInfo info;
    Subprocess::FinishStatus finishStatus = process->getFinishStatus();
    if (finishStatus == Subprocess::NO_EXIT) {
        return false;
    }

    auto timeTicksElapsed
      = 1.0 * process->elapsedTime().count() / GetTimeTickLength().count();

    if (finishStatus == Subprocess::NON_ZERO_EXIT) {
        info.fail("Test exited with code "
                    + std::to_string(process->getReturnCode()),
                  timeTicksElapsed);
    } else if (finishStatus == Subprocess::SIGNAL_EXIT) {
        info.fail("Test killed by signal "
                    + std::to_string(process->getSignal()),
                  timeTicksElapsed);
    } else if (finishStatus == Subprocess::TIMEOUT) {
        info.fail("Test execution timed out.", timeTicksElapsed);
    } else if (finishStatus == Subprocess::ZERO_EXIT) {
        while (true) {
            Message message = process->getNextMessage(1);
            if (message.isInvalid()) {
                info.fail("Unexpected 0-code exit.", timeTicksElapsed);
                break;
            }
            auto messageType = message.read<PipeMessageType>();
            if (messageType != PipeMessageType::DONE) {
                // It's a warning
                onWarning(Warning(message.read<std::string>(),
                                  test.getGroup()->getId(),
                                  test.getId()));
                continue;
            }
            message >> info.timeTicks >> info.passed >> info.failure;
            break;
        }
    }
    test.addExecution(info);
    onTestExecutionFinish(test);
    if (!test.isExecuted()) {
        onTestExecutionStart(test);
        box->second = this->startExecution(test);
        return false;
    }
    return true;
}

void BoxExecutor::ensureEmptyBoxes(size_t requiredEmpty) {
    while (activeBoxes.size() > numBoxes - requiredEmpty) {
        activeBoxes.erase(
          remove_if(activeBoxes.begin(),
                    activeBoxes.end(),
                    [this](Box& box) { return this->tryCloseBox(&box); }),
          activeBoxes.end());
        std::this_thread::sleep_for(loopSleepTime);
    }
}

}  // namespace mcga::test
