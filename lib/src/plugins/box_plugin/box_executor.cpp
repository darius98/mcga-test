#include <iostream>

#include <EasyFlags.hpp>

#include "box_executor.hpp"

using namespace autojson;
using namespace std;


namespace kktest {

TestContainer::TestContainer(const string& binaryPath):
        box(binaryPath), test(nullptr) {}

void TestContainer::runTest(Test* _test) {
    test = _test;
    box.run("-qt " + to_string(test->getIndex()));
}

bool TestContainer::tryFinalize(function<void(Test*, const string&)> callback) {
    if (!box.poll()) {
        return false;
    }
    if (test == nullptr) {
        return true;
    }
    callback(test, box.getOutput());
    test = nullptr;
    return true;
}

BoxExecutor::BoxExecutor(const string& binaryPath, int numBoxes) {
    for (int i = 0; i < numBoxes; ++ i) {
        containers.emplace_back(binaryPath);
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
    return container.tryFinalize([this](Test* test, const string& procOutput) {
        test->loadFromJSON(JSON::parse(procOutput));
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
