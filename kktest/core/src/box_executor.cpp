#include "kktest/core/src/box_executor.hpp"

#include "common/interproc/src/pipe.hpp"
#include "common/utils/src/time.hpp"
#include "kktest/core/src/errors.hpp"

using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;
using namespace std::placeholders;

// TODO(darius98): This implementation required refactoring.

namespace kktest {

enum MessageStatus: uint8_t {
    SUCCESS = 0,
    CONFIGURATION_ERROR = 1
};

BoxExecutor::BoxExecutor(OnTestFinished onTestFinished, size_t numBoxes):
        Executor(move(onTestFinished)), numBoxes(numBoxes) {}

void BoxExecutor::execute(Test&& test, const Executable& func) {
    ensureEmptyBoxes(1);
    double timeLimit = test.getTimeTicksLimit() * getTimeTickLengthMs() + 100.0;
    GroupPtr group = test.getGroup();
    auto process = new WorkerSubprocess(
            Duration::fromMs(timeLimit),
            bind(&BoxExecutor::runBoxed, this, group, func, _1));
    activeBoxes.push_back(
        BoxedTest{move(test), func, {}, unique_ptr<WorkerSubprocess>(process)});
}

void BoxExecutor::runBoxed(GroupPtr group,
                           const Executable& func,
                           PipeWriter* pipe) {
    try {
        ExecutedTest::Info info = run(move(group), func);
        pipe->sendMessage(SUCCESS, info.timeTicks, info.passed, info.failure);
    } catch(const ConfigurationError& error) {
        pipe->sendMessage(CONFIGURATION_ERROR, string(error.what()));
    }
}

void BoxExecutor::finalize() {
    ensureEmptyBoxes(numBoxes);
}

void BoxExecutor::ensureEmptyBoxes(size_t requiredEmpty) {
    size_t maxActive = numBoxes - requiredEmpty;
    while (activeBoxes.size() > maxActive) {
        bool progress = false;
        for (auto it = activeBoxes.begin(); it != activeBoxes.end(); ) {
            if (tryCloseBox(it)) {
                if (it->executions.size() == it->test.getNumAttempts()) {
                    onTestFinished(ExecutedTest(move(it->test),
                                                move(it->executions)));
                    it = activeBoxes.erase(it);
                } else {
                    double timeLimit = it->test.getTimeTicksLimit()
                                            * getTimeTickLengthMs() + 100.0;
                    GroupPtr group = it->test.getGroup();
                    it->process.reset(new WorkerSubprocess(
                        Duration::fromMs(timeLimit),
                        bind(&BoxExecutor::runBoxed,
                             this,
                             group,
                             it->testFunc,
                             _1)));
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

bool BoxExecutor::tryCloseBox(vector<BoxedTest>::iterator boxedTest) {
    bool passed = false;
    Message message;
    string error;
    auto process = boxedTest->process.get();
    switch (process->getFinishStatus()) {
        case WorkerSubprocess::NO_EXIT: {
            return false;
        }
        case WorkerSubprocess::ZERO_EXIT: {
            message = process->getNextMessage(32);
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
                    + to_string(process->getReturnCode()) + ".";
            break;
        }
        case WorkerSubprocess::SIGNAL_EXIT: {
            error = "Test killed by signal " + to_string(process->getSignal());
            break;
        }
        case WorkerSubprocess::TIMEOUT: {
            error = "Test execution timed out.";
            break;
        }
    }
    if (!passed) {
        boxedTest->executions.emplace_back(move(error));
        return true;
    }
    if (message.read<MessageStatus>() == CONFIGURATION_ERROR) {
        // Read the error message and throw it.
        throw ConfigurationError(message.read<string>());
    }
    ExecutedTest::Info info;
    message >> info.timeTicks >> info.passed >> info.failure;
    boxedTest->executions.push_back(move(info));
    return true;
}

}
