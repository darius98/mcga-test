#include "core/src/box_executor.hpp"

#include "common/interproc/src/pipe.hpp"
#include "common/utils/src/process_time.hpp"
#include "core/src/errors.hpp"

using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;
using namespace std::placeholders;

namespace kktest {

BoxedTest::BoxedTest(Test* _test, WorkerSubprocess* _process):
        test(_test), process(_process) {}

BoxedTest::BoxedTest(BoxedTest&& other) noexcept:
        test(other.test), process(other.process) {
    other.process = nullptr;
}

BoxedTest::~BoxedTest() {
    delete process;
}

bool BoxedTest::operator<(const BoxedTest& other) const {
    return test < other.test;
}

BoxExecutor::BoxExecutor(const OnTestFinished& onTestFinished,
                         size_t _maxNumContainers):
        Executor(onTestFinished),
        maxNumContainers(_maxNumContainers) {}

void BoxExecutor::execute(Test* test, Executable func) {
    ensureFreeContainers(1);
    openContainers.insert(BoxedTest(test, new WorkerSubprocess(
        test->getConfig().timeTicksLimit * getTimeTickLengthMs() + 100.0,
        bind(&BoxExecutor::runContained, this, test, func, _1))));
}

void BoxExecutor::runContained(Test* test, Executable func, PipeWriter* pipe) {
    try {
        auto executionInfo = run(test, func);
        pipe->sendMessage(executionInfo.toMessage());
    } catch(const ConfigurationError& error) {
        pipe->sendMessage(TestRun::CONFIGURATION_ERROR, string(error.what()));
    }
}

void BoxExecutor::finalize() {
    ensureFreeContainers(maxNumContainers);
}

void BoxExecutor::ensureFreeContainers(size_t numContainers) {
    while (openContainers.size() > maxNumContainers - numContainers) {
        bool progress = false;
        for (auto it = openContainers.begin(); it != openContainers.end(); ) {
            if (tryCloseContainer(it)) {
                it = openContainers.erase(it);
                progress = true;
            } else {
                ++ it;
            }
        }
        if (!progress) {
            sleepForDuration(Duration::fromMs(5));
        }
    }
}

bool BoxExecutor::tryCloseContainer(set<BoxedTest>::iterator boxedTest) {
    bool finished = true;
    bool passed = false;
    Message message;
    string error;
    auto process = boxedTest->process;
    switch (process->getFinishStatus()) {
        case WorkerSubprocess::NO_EXIT: {
            finished = false;
            break;
        }
        case WorkerSubprocess::ZERO_EXIT: {
            passed = true;
            message = process->getNextMessage(32);
            if (message.isInvalid()) {
                passed = false;
                error = "Unexpected 0-code exit.";
            }
            break;
        }
        case WorkerSubprocess::NON_ZERO_EXIT: {
            error = "Test exited with code "
                    + to_string(process->getReturnCode()) + ".";
            break;
        }
        case WorkerSubprocess::SIGNAL_EXIT: {
            error = "Test killed by signal "
                    + to_string(process->getSignal()) + ".";
            break;
        }
        case WorkerSubprocess::TIMEOUT: {
            error = "Test execution timed out.";
            break;
        }
    }
    if (!finished) {
        return false;
    }
    auto info = passed
            ? TestRun(boxedTest->test, message)
            : TestRun(boxedTest->test, error);
    onTestFinishedCallback(info);
    return true;
}

}
