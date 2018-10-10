#include <EasyFlags.hpp>
#include <JSON>

#include "box_executor.hpp"

using namespace autojson;
using namespace std;

AddArgument(string, argumentBoxId)
    .ArgumentType("string")
    .Name("box-id")
    .Short("b")
    .Description("ID of the box to use for boxed testing.")
    .DefaultValue("0");


namespace kktest {

BoxExecutor::BoxExecutor(int testIndexToRun, string binaryPath):
        Executor(testIndexToRun),
        box(new BoxWrapper(argumentBoxId, binaryPath)) {}

BoxExecutor::~BoxExecutor() {
    delete box;
}

void BoxExecutor::checkIsInactive(const string& methodName) const {}

bool BoxExecutor::isDuringTest() const {
    return false;
}

void BoxExecutor::execute(const vector<Group*>& groups,
                          Test* test,
                          Executable func,
                          int testIndex) {
    box->run("-s -t " + to_string(testIndex));
    while (!box->poll()) {}
    pair<string, JSON> boxRunStats = box->getRunStats();
    if ((int)boxRunStats.second["exitCode"] != 0) {
        test->setFailure(boxRunStats.first);
    }
}

}
