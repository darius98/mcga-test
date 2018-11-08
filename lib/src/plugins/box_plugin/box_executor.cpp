#include "box_executor.hpp"

using namespace std;


namespace kktest {

BoxExecutor::BoxExecutor(std::size_t _maxNumContainers):
        maxNumContainers(_maxNumContainers) {}

void BoxExecutor::execute(Test* test, Executable func, Executable after) {
    ensureFreeContainers(1);
    openContainers.insert(new TestContainer(test, [func, test, this]() {
        run(test, func);
    }, after));
}

void BoxExecutor::finalize() {
    ensureFreeContainers(maxNumContainers);
}

void BoxExecutor::ensureFreeContainers(size_t numContainers) {
    while (openContainers.size() > maxNumContainers - numContainers) {
        for (auto it = openContainers.begin(); it != openContainers.end(); ) {
            if ((*it)->isTestFinished()) {
                delete (*it);
                it = openContainers.erase(it);
            } else {
                ++ it;
            }
        }
    }
}

}
