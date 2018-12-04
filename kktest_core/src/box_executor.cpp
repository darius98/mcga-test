#include <kktest_common/interproc.hpp>
#include <kktest_common/time.hpp>

#include "box_executor.hpp"

using std::size_t;
using kktest::interproc::Message;
using kktest::interproc::MessageReader;
using kktest::interproc::PipeWriter;
using kktest::utils::sleepForMs;

namespace kktest {

BoxExecutor::BoxExecutor(size_t _maxNumContainers): maxNumContainers(_maxNumContainers) {}

void BoxExecutor::execute(Test* test, Executable func) {
    ensureFreeContainers(1);
    openContainers.insert(new TestContainer(
        test->getConfig().timeTicksLimit * getTimeTickLengthMs() + 100.0,
        [this, func, test](PipeWriter* pipe) {
            onTestFinished([](Test*) {});
            run(test, func);
            pipe->sendMessage(Message::build(test->isPassed(),
                                             test->getExecutionTimeTicks(),
                                             test->getFailureMessage()));
        },
        [this, test](const Message& message) {
            MessageReader reader(message);
            auto isPassed = reader.read<bool>();
            auto ticks = reader.read<double>();
            auto failureMessage = reader.read<String>();
            setTestExecuted(test, ticks, isPassed, failureMessage);
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
