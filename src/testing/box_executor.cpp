#include <EasyFlags.hpp>
#include <JSON>

#include "box_executor.hpp"

using namespace autojson;
using namespace std;

AddArgument(string, boxId)
    .ArgumentType("string")
    .Name("box-id")
    .Short("b")
    .Description("ID of the box to use for boxed testing.")
    .DefaultValue("0");


namespace kktest {

BoxExecutor::BoxExecutor(int testIndexToRun, string _binaryPath):
        Executor(testIndexToRun),
        binaryPath(move(_binaryPath)),
        copiedBinary(false) {}

void BoxExecutor::checkIsInactive(const string& methodName) const {}

bool BoxExecutor::isDuringTest() const {
    return false;
}

void BoxExecutor::execute(const vector<Group*>& groups,
                          Test* test,
                          Executable func,
                          int testIndex) {
    string boxDir = "/tmp/box/" + boxId + "/box/";

    if (!copiedBinary) {
        system(("cp " + binaryPath + " " + boxDir + "test").c_str());
        copiedBinary = true;
    }

    string boxedProcess = "box --run --meta=" + boxId + " --box-id=" + boxId
                          + " -- ./test -s -t " + to_string(testIndex);

    FILE* pipe = popen(boxedProcess.c_str(), "r");
    if (!pipe) {
        throw runtime_error("popen() failed!");
    }
    string testOutput;
    char buffer[32];
    while (!feof(pipe)) {
        if (fgets(buffer, 32, pipe) != nullptr) {
            testOutput += buffer;
        }
    }

    JSON runStats = JSON::readFromFile(boxId);
    system(("rm " + boxId).c_str());
    if (pclose(pipe) != 0 || (int)runStats["exitCode"] != 0) {
        test->setFailure(testOutput);
    }
}

}
