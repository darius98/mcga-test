#include <iostream>

#include <EasyFlags.hpp>

#include "box_executor.hpp"

using namespace autojson;
using namespace std;


namespace kktest {

TestContainer::TestContainer(const string& boxId, const string& binaryPath):
        box(boxId, binaryPath), test(nullptr) {}

void TestContainer::runTest(Test* _test) {
    test = _test;
    box.run("-qt " + to_string(test->getIndex()));
}

bool TestContainer::tryFinalize(function<void(Test*, JSON)> callback) {
    if (!box.poll()) {
        return false;
    }
    if (test == nullptr) {
        return true;
    }
    callback(test, box.getRunStats());
    test = nullptr;
    return true;
}

BoxExecutor::BoxExecutor(const string& binaryPath,
                         int boxIdBegin,
                         int boxIdEnd) {
    for (int boxId = boxIdBegin; boxId < boxIdEnd; ++ boxId) {
        containers.emplace_back(to_string(boxId), binaryPath);
    }
}

void BoxExecutor::checkIsInactive(const string&) const {}

bool BoxExecutor::isDuringTest() const {
    return false;
}

void BoxExecutor::execute(Test* test, Executable) {
    findEmptyContainer().runTest(test);
}

TestContainer& BoxExecutor::findEmptyContainer() {
    while(true) {
        for (TestContainer &container : containers) {
            if (tryFinalizeContainer(container)) {
                return container;
            }
        }
    }
}

bool BoxExecutor::tryFinalizeContainer(TestContainer& container) {
    return container.tryFinalize([this](Test* test, JSON boxRunStats) {
        test->loadFromJSON(JSON::parse(boxRunStats["processOutput"]));
        afterTest(test);
    });
}

void BoxExecutor::finalize() {
    bool allFinalized = false;
    while (!allFinalized) {
        allFinalized = true;
        for (TestContainer& container: containers) {
            if (!tryFinalizeContainer(container)) {
                allFinalized = false;
            }
        }
    }
}

}