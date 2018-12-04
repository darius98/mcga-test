#include <kktest_common/interproc.hpp>
#include <kktest_common/time.hpp>

#include "box_executor.hpp"

using kktest::interproc::Message;
using kktest::interproc::MessageReader;
using kktest::interproc::PipeWriter;
using kktest::utils::sleepForMs;
using std::function;
using std::get;
using std::size_t;

namespace kktest {

BoxExecutor::BoxExecutor(const function<void(Test*)>& onTestFinishedCallback,
                         size_t _maxNumContainers):
        Executor(onTestFinishedCallback),
        maxNumContainers(_maxNumContainers) {}

void BoxExecutor::execute(Test* test, Executable func) {
    ensureFreeContainers(1);
    openContainers.insert(new TestContainer(
        test->getConfig().timeTicksLimit * getTimeTickLengthMs() + 100.0,
        [this, func, test](PipeWriter* pipe) {
            auto results = run(test, func);
            test->setExecuted(get<0>(results), get<1>(results), get<2>(results));
            pipe->sendMessage(test->isPassed(),
                              test->getExecutionTimeTicks(),
                              test->getFailureMessage());
        },
        [this, test](const Message& message) {
            MessageReader reader(message);
            auto isPassed = reader.read<bool>();
            auto ticks = reader.read<double>();
            auto failureMessage = reader.read<String>();
            test->setExecuted(ticks, isPassed, failureMessage);
            onTestFinishedCallback(test);
        }));
}

void BoxExecutor::finalize() {
    ensureFreeContainers(maxNumContainers);
}

void BoxExecutor::ensureFreeContainers(size_t numContainers) {
    while (openContainers.size() > maxNumContainers - numContainers) {
        bool progressed = false;
        for (auto it = openContainers.begin(); it != openContainers.end(); ) {
            if ((*it)->poll()) {
                progressed = true;
                delete *it;
                it = openContainers.erase(it);
            } else {
                ++it;
            }
        }
        if (!progressed) {
            sleepForMs(10);
        }
    }
}

}  // namespace kktest
