#include <iostream>
#include <sstream>

#include <EasyFlags.hpp>
#include <JSON>

#include "box_executor.hpp"

using namespace autojson;
using namespace std;

AddArgument(string, argumentBoxes)
    .ArgumentType("comma separated string list")
    .Name("boxes")
    .Short("b")
    .Description("Array of box ids (integers) to be used while running boxed.")
    .DefaultValue("0,1,2,3,4,5,6,7,8,9");


namespace kktest {

BoxExecutor::BoxExecutor(int testIndexToRun, bool verbose, string binaryPath):
        Executor(testIndexToRun, verbose) {
    stringstream stream(argumentBoxes);
    string boxId;
    while (getline(stream, boxId, ',')) {
        boxes.emplace_back(new BoxWrapper(boxId, binaryPath), nullptr);
    }
}

BoxExecutor::~BoxExecutor() {
    for (auto p: boxes) {
        delete p.first;
    }
}

void BoxExecutor::checkIsInactive(const string& methodName) const {}

bool BoxExecutor::isDuringTest() const {
    return false;
}

void BoxExecutor::execute(const vector<Group*>& groups,
                          Test* test,
                          Executable func) {
    int emptyBoxIndex = pollForEmptyBox();
    boxes[emptyBoxIndex].second = test;
    boxes[emptyBoxIndex].first->run("-s --test=" + to_string(test->getIndex()));
}

int BoxExecutor::pollForEmptyBox() {
    while (!tryFinalizeBox(currentBoxIndex)) {
        currentBoxIndex += 1;
        if (currentBoxIndex == (int)boxes.size()) {
            currentBoxIndex = 0;
        }
    }
    return currentBoxIndex;
}

bool BoxExecutor::tryFinalizeBox(int boxIndex) {
    if (!boxes[boxIndex].first->poll()) {
        return false;
    }
    if (boxes[boxIndex].second == nullptr) {
        return true;
    }
    pair<string, JSON> boxRunStats = boxes[boxIndex].first->getRunStats();
    if ((int)boxRunStats.second["exitCode"] != 0) {
        boxes[boxIndex].second->setFailure(boxRunStats.first);
    }
    logTest(boxes[boxIndex].second);
    boxes[boxIndex].second->updateGroups();
    boxes[boxIndex].second = nullptr;
    return true;
}

void BoxExecutor::finalize() {
    for (int i = 0; i < (int)boxes.size(); ++ i) {
        while (!tryFinalizeBox(i)) {
        }
    }
}

}
