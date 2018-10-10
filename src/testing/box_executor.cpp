#include <EasyFlags.hpp>
#include <JSON>

#include "box_executor.hpp"

using namespace autojson;
using namespace std;

AddArgument(string, boxId).Name("box-id").DefaultValue("0");


namespace kktest {

BoxExecutor::BoxExecutor(string _binaryPath): binaryPath(move(_binaryPath)) {}

void BoxExecutor::checkIsInactive(const string& methodName) const {}

bool BoxExecutor::isDuringTest() const {
    return false;
}

void BoxExecutor::execute(const vector<Group*>& groups,
                          Test* test,
                          Executable func) {
    string boxDir = "/tmp/box/" + boxId + "/box/";

    if (!copiedBinary) {
        system(("cp " + binaryPath + " " + boxDir + "test").c_str());
        copiedBinary = true;
    }

    string processName = "box --run --meta=" + boxId + " --box-id=" + boxId;
    processName += " -- ./test --smooth --enable-logging=0 --single-test=";
    processName += to_string(getCurrentTestIndex());

    FILE* pipe = popen(processName.c_str(), "r");
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
