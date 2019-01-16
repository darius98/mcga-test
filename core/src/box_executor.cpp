#include <kktest_common/interproc.hpp>

#include "box_executor.hpp"
#include "errors.hpp"

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
    openContainers.insert(new TestContainer(
        test->getConfig().timeTicksLimit * getTimeTickLengthMs() + 100.0,
        [this, func, test](PipeWriter* pipe) {
            try {
                auto executionInfo = run(test, func);
                pipe->sendMessage(executionInfo.toMessage());
            } catch(const ConfigurationError& error) {
                pipe->sendMessage(TestExecutionInfo::CONFIGURATION_ERROR,
                                  String(error.what()));
            }
        },
        [this, test](const Message& message) {
            test->setExecuted(TestExecutionInfo::fromMessage(message));
            onTestFinishedCallback(test);
        }));
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
