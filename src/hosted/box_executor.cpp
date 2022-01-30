#include "box_executor.hpp"

#include <algorithm>
#include <thread>

#include "core/time_tick.hpp"
#include "serialization.hpp"

using mcga::proc::Message;
using mcga::proc::PipeWriter;
using mcga::proc::Subprocess;
using mcga::proc::WorkerSubprocess;

namespace mcga::test {

enum class PipeMessageType : char {
    WARNING,
    DONE,
};

BoxExecutor::BoxExecutor(size_t numBoxes): Executor(BOXED), numBoxes(numBoxes) {
}

void BoxExecutor::execute(Test test) {
    ensureEmptyBoxes(1);
    beforeTestExecution(test);
    if (!test.isFinished()) {
        auto execProcess = startExecution(test);
        activeBoxes.emplace_back(std::move(test), std::move(execProcess));
    }
}

void BoxExecutor::finalize() {
    ensureEmptyBoxes(numBoxes);
}

void BoxExecutor::emitWarning(Warning warning, GroupPtr group) {
    if (isActive()) {
        decorateWarningWithCurrentTestNotes(warning, group);
        currentTestingSubprocessPipe->sendMessage(PipeMessageType::WARNING,
                                                  warning);
    } else {
        onWarning(std::move(warning), group);
    }
}

void BoxExecutor::executeBoxed(const Test& test,
                               std::unique_ptr<PipeWriter> pipe) {
    currentTestingSubprocessPipe = std::move(pipe);
    Test::ExecutionInfo info = run(test);
    currentTestingSubprocessPipe->sendMessage(PipeMessageType::DONE,
                                              info.status,
                                              info.verb,
                                              info.message,
                                              info.context,
                                              info.timeTicks);
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

    info.timeTicks = NanosecondsToTimeTicks(process->elapsedTime());

    if (finishStatus == Subprocess::NON_ZERO_EXIT) {
        info.fail("Test exited with code "
                    + std::to_string(process->getReturnCode()),
                  std::nullopt);
    } else if (finishStatus == Subprocess::SIGNAL_EXIT) {
        info.fail("Test killed by signal "
                    + std::to_string(process->getSignal()),
                  std::nullopt);
    } else if (finishStatus == Subprocess::TIMEOUT) {
        info.fail("Test execution timed out.", std::nullopt);
    } else if (finishStatus == Subprocess::ZERO_EXIT) {
        while (true) {
            Message message = process->getNextMessage(1);
            if (message.isInvalid()) {
                info.fail("Unexpected 0-code exit.", std::nullopt);
                break;
            }
            auto messageType = message.read<PipeMessageType>();
            if (messageType != PipeMessageType::DONE) {
                // It's a warning
                onWarning(message.read<Warning>(), nullptr);
                continue;
            }
            message >> info.status >> info.verb >> info.message >> info.context
              >> info.timeTicks;
            break;
        }
    }
    test.addExecution(info, api);
    if (!test.isFinished()) {
        beforeTestExecution(test);
        if (!test.isFinished()) {
            box->second = startExecution(test);
            return false;
        }
    }
    return true;
}

void BoxExecutor::ensureEmptyBoxes(size_t requiredEmpty) {
    while (activeBoxes.size() > numBoxes - requiredEmpty) {
        activeBoxes.erase(remove_if(activeBoxes.begin(),
                                    activeBoxes.end(),
                                    [this](Box& box) {
                                        return tryCloseBox(&box);
                                    }),
                          activeBoxes.end());
        std::this_thread::sleep_for(loopSleepTime);
    }
}

}  // namespace mcga::test
