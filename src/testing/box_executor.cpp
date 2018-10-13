#include <iostream>
#include <sstream>

#include <EasyFlags.hpp>

#include "box_executor.hpp"

using namespace autojson;
using namespace std;

AddArgument(string, argumentBoxes)
    .ArgumentType("comma separated string list")
    .Name("box_ids")
    .Description("Array of box ids (integers) to be used while running boxed.")
    .DefaultValue("0,1,2,3,4,5,6,7,8,9");


namespace kktest {

BoxExecutor::BoxExecutor(int testIndexToRun, string binaryPath):
        Executor(testIndexToRun) {
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
    boxes[emptyBoxIndex].first->run("-qt " + to_string(test->getIndex()));
}

int BoxExecutor::pollForEmptyBox() {
    for (int i = 0; i < (int)boxes.size(); ++ i) {
        tryFinalizeBox(i);
    }
    while (!tryFinalizeBox(currentBoxIndex)) {
        currentBoxIndex += 1;
        if (currentBoxIndex == (int)boxes.size()) {
            currentBoxIndex = 0;
        }
    }
    return currentBoxIndex;
}

bool BoxExecutor::tryFinalizeBox(int boxIndex) {
    pair<BoxWrapper*, Test*>& box = boxes[boxIndex];
    if (!box.first->poll()) {
        return false;
    }
    if (box.second == nullptr) {
        return true;
    }
    JSON boxRunStats = box.first->getRunStats();
    if ((int)boxRunStats["exitCode"] != 0) {
        string failureMessage = boxRunStats["processOutput"];
        box.second->setFailure(failureMessage);
    }
    afterTest(box.second);
    box.second = nullptr;
    return true;
}

void BoxExecutor::finalize() {
    bool allFinalized = false;
    while (!allFinalized) {
        allFinalized = true;
        for (int i = 0; i < (int)boxes.size(); ++ i) {
            if (!tryFinalizeBox(i)) {
                allFinalized = false;
            }
        }
    }
}

}
