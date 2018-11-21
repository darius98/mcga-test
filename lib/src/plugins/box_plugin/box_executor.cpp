#include "box_executor.hpp"

using namespace std;


namespace kktest {

BoxExecutor::BoxExecutor(size_t _maxNumContainers): maxNumContainers(_maxNumContainers) {}

void BoxExecutor::execute(Test* test, Executable func) {
    ensureFreeContainers(1);
    openContainers.insert(new TestContainer(
        test,
        test->getConfig().timeTicksLimit * getTimeTickLengthMs() + 100.0,
        [this, func, test]() {
            onTestFinished([](Test*) {});
            run(test, func);
        }
    ));
}

void BoxExecutor::finalize() {
    ensureFreeContainers(maxNumContainers);
}

void BoxExecutor::ensureFreeContainers(size_t numContainers) {
    while (openContainers.size() > maxNumContainers - numContainers) {
        for (auto it = openContainers.begin(); it != openContainers.end(); ) {
            if ((*it)->isTestFinished()) {
                auto container = *it;
                setTestExecuted(container->getTest(),
                                container->getTicks(),
                                container->isPassed(),
                                container->getFailureMessage());
                delete container;
                it = openContainers.erase(it);
            } else {
                ++ it;
            }
        }
    }
}

}
