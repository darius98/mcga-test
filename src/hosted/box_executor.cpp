#include "box_executor.hpp"

#include <algorithm>
#include <thread>

#include "core/time_tick.hpp"
#include "core/serialization.hpp"

namespace mcga::test {

void read_custom(proc::binary_reader auto& reader, Test::ExecutionInfo& info) {
    proc::read_into(reader,
                    info.status,
                    info.verb,
                    info.message,
                    info.context,
                    info.timeTicks);
}

void write_custom(proc::binary_reader auto& writer,
                const Test::ExecutionInfo& info) {
    proc::write_from(writer,
                     info.status,
                     info.verb,
                     info.message,
                     info.context,
                     info.timeTicks);
}

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
                               std::unique_ptr<proc::PipeWriter> pipe) {
    currentTestingSubprocessPipe = std::move(pipe);
    Test::ExecutionInfo info = run(test);
    currentTestingSubprocessPipe->sendMessage(PipeMessageType::DONE, info);
}

std::unique_ptr<proc::WorkerSubprocess>
  BoxExecutor::startExecution(const Test& test) {
    auto timeLimit = TimeTicksToNanoseconds(test.getTimeTicksLimit())
      + std::chrono::seconds(1);
    return make_unique<proc::WorkerSubprocess>(
      timeLimit, [this, &test](std::unique_ptr<proc::PipeWriter> pipe) {
          executeBoxed(test, std::move(pipe));
      });
}

bool BoxExecutor::tryCloseBox(Box* box) {
    Test& test = box->first;
    proc::WorkerSubprocess* process = box->second.get();

    Test::ExecutionInfo info;
    proc::Subprocess::FinishStatus finishStatus = process->getFinishStatus();
    if (finishStatus == proc::Subprocess::NO_EXIT) {
        return false;
    }

    info.timeTicks = NanosecondsToTimeTicks(process->elapsedTime());

    if (finishStatus == proc::Subprocess::NON_ZERO_EXIT) {
        info.fail("Test exited with code "
                    + std::to_string(process->getReturnCode()),
                  std::nullopt);
    } else if (finishStatus == proc::Subprocess::SIGNAL_EXIT) {
        info.fail("Test killed by signal "
                    + std::to_string(process->getSignal()),
                  std::nullopt);
    } else if (finishStatus == proc::Subprocess::TIMEOUT) {
        info.fail("Test execution timed out.", std::nullopt);
    } else if (finishStatus == proc::Subprocess::ZERO_EXIT) {
        while (true) {
            auto message = process->getNextMessage(1);
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
            info = message.read<Test::ExecutionInfo>();
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
