#include "core/src/box_executor.hpp"

#include "common/interproc/src/pipe.hpp"
#include "common/utils/src/time.hpp"
#include "core/src/errors.hpp"

using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;

namespace kktest {

BoxedTest::BoxedTest(Test* _test, interproc::WorkerSubprocess* _process):
        test(_test), process(_process) {}

BoxedTest::BoxedTest(kktest::BoxedTest&& other) noexcept:
        test(other.test), process(other.process) {
    other.process = nullptr;
}

BoxedTest::~BoxedTest() {
    delete process;
}

bool BoxedTest::operator<(const BoxedTest& other) const {
    return test < other.test;
}

BoxExecutor::BoxExecutor(const OnTestFinishedCallback& onTestFinishedCallback,
                         size_t _maxNumContainers):
        Executor(onTestFinishedCallback),
        maxNumContainers(_maxNumContainers) {}

void BoxExecutor::execute(Test* test, Executable func) {
    ensureFreeContainers(1);
    openContainers.insert(BoxedTest(test, new WorkerSubprocess(
        test->getConfig().timeTicksLimit * getTimeTickLengthMs() + 100.0,
        bind(&BoxExecutor::runContained, this, test, func, placeholders::_1))));
}

void BoxExecutor::runContained(Test* test, Executable func, PipeWriter* pipe) {
    try {
        auto executionInfo = run(test, func);
        pipe->sendMessage(executionInfo.toMessage());
    } catch(const ConfigurationError& error) {
        pipe->sendMessage(TestExecutionInfo::CONFIGURATION_ERROR,
                          string(error.what()));
    }
}

void BoxExecutor::finalize() {
    ensureFreeContainers(maxNumContainers);
}

void BoxExecutor::ensureFreeContainers(size_t numContainers) {
    while (openContainers.size() > maxNumContainers - numContainers) {
        bool progress = false;
        auto it = openContainers.begin();
        while (it != openContainers.end()) {
            bool finished = true;
            bool failed = true;
            Message message;
            string error;
            switch (it->process->getFinishStatus()) {
                case WorkerSubprocess::NO_EXIT: {
                    finished = false;
                    break;
                }
                case WorkerSubprocess::ZERO_EXIT: {
                    failed = false;
                    message = it->process->getNextMessage(32);
                    if (message.isInvalid()) {
                        failed = true;
                        error = "Unexpected 0-code exit.";
                    }
                    break;
                }
                case WorkerSubprocess::NON_ZERO_EXIT: {
                    error = "Test exited with code "
                            + to_string(it->process->getReturnCode())
                            + ".";
                    break;
                }
                case WorkerSubprocess::SIGNAL_EXIT: {
                    error = "Test killed by signal "
                            + to_string(it->process->getSignal())
                            + ".";
                    break;
                }
                case WorkerSubprocess::TIMEOUT: {
                    error = "Test execution timed out.";
                    break;
                }
            }
            if (!finished) {
                ++ it;
                continue;
            }
            if (failed) {
                it->test->setExecuted(TestExecutionInfo::fromError(error));
            } else {
                it->test->setExecuted(TestExecutionInfo::fromMessage(message));
            }
            onTestFinishedCallback(it->test);
            it = openContainers.erase(it);
            progress = true;
        }
        if (!progress) {
            sleepForMs(5);
        }
    }
}

}
