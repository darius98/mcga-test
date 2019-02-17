#include "core/src/box_executor.hpp"

#include "common/interproc/src/pipe.hpp"
#include "common/utils/src/time.hpp"
#include "core/src/errors.hpp"

using namespace kktest::interproc;
using namespace kktest::utils;
using namespace std;

namespace kktest {

BoxExecutor::BoxExecutor(const OnTestFinishedCallback& onTestFinishedCallback,
                         size_t _maxNumContainers):
        Executor(onTestFinishedCallback),
        maxNumContainers(_maxNumContainers) {}

void BoxExecutor::execute(Test* test, Executable func) {
    ensureFreeContainers(1);
    openContainers.insert(new WorkerSubprocess(
        test->getConfig().timeTicksLimit * getTimeTickLengthMs() + 100.0,
        bind(&BoxExecutor::runContained, this, test, func, placeholders::_1),
        bind(&BoxExecutor::onContainerMessage, this, test, placeholders::_1)));
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

void BoxExecutor::onContainerMessage(Test* test, Message& message) {
    WorkerSubprocess::ResultType t;
    message >> t;
    if (t == WorkerSubprocess::ERROR) {
        string errorMessage;
        message >> errorMessage;
        test->setExecuted(TestExecutionInfo::fromError(errorMessage));
    } else {
        auto executionInfo = TestExecutionInfo::fromMessage(message);
        test->setExecuted(executionInfo);
        onTestFinishedCallback(test);
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
            if ((*it)->poll()) {
                delete *it;
                it = openContainers.erase(it);
                progress = true;
                continue;
            }
            ++it;
        }
        if (!progress) {
            sleepForMs(5);
        }
    }
}

}
