#include "box_executor.hpp"

#include <algorithm>
#include <thread>

#include "time_tick.hpp"

using mcga::proc::Message;
using mcga::proc::PipeWriter;
using mcga::proc::Subprocess;
using mcga::proc::WorkerSubprocess;
using std::make_unique;
using std::move;
using std::operator""s;
using std::remove_if;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::this_thread::sleep_for;

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
    activeBoxes.emplace_back(move(test), move(firstExecution));
}

void BoxExecutor::finalize() {
    ensureEmptyBoxes(numBoxes);
}

Executor::Type BoxExecutor::getType() const {
    return BOXED;
}

void BoxExecutor::emitWarning(const string& message, std::size_t /*groupId*/) {
    if (isActive()) {
        currentTestingSubprocessPipe->sendMessage(WARNING, message);
    }
}

void BoxExecutor::executeBoxed(const Test& test, PipeWriter* pipe) {
    currentTestingSubprocessPipe = pipe;
    Test::ExecutionInfo info = run(test);
    pipe->sendMessage(DONE, info.timeTicks, info.passed, info.failure);
}

unique_ptr<WorkerSubprocess> BoxExecutor::startExecution(const Test& test) {
    auto timeLimit = TimeTicksToNanoseconds(test.getTimeTicksLimit()) + 1s;
    return make_unique<WorkerSubprocess>(
      timeLimit, [this, &test](PipeWriter* pipe) { executeBoxed(test, pipe); });
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
                    + to_string(process->getReturnCode()),
                  timeTicksElapsed);
    } else if (finishStatus == Subprocess::SIGNAL_EXIT) {
        info.fail("Test killed by signal " + to_string(process->getSignal()),
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
                onWarning(Warning(message.read<string>(),
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
        sleep_for(loopSleepTime);
    }
}

}  // namespace mcga::test
