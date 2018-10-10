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

BoxWrapper::BoxWrapper(string _boxId, string _binaryPath):
    boxId(move(_boxId)),
    binaryPath(move(_binaryPath)),
    copiedBinary(false),
    available(true) {}

void BoxWrapper::run(int testIndex) {
    if (!available) {
        throw runtime_error("Used un-available box!");
    }
    runStatsAvailable = false;
    available = false;

    string boxDir = "/tmp/box/" + boxId + "/box/";

    if (!copiedBinary) {
        system(("mkdir -p " + boxDir).c_str());
        system(("cp " + binaryPath + " " + boxDir + "test").c_str());
        copiedBinary = true;
    }

    processOutput.clear();

    string boxedProcess = "box --run --meta=" + boxId + " --box-id=" + boxId
                          + " -- ./test -s -t " + to_string(testIndex);
    processFileDescriptor = popen(boxedProcess.c_str(), "r");
    if (!processFileDescriptor) {
        throw runtime_error("popen() failed!");
    }
}

bool BoxWrapper::poll() {
    if (available) {
        return true;
    }

    if (fgets(processOutputReadBuffer, 32, processFileDescriptor) != nullptr) {
        processOutput += processOutputReadBuffer;
    }
    if (feof(processFileDescriptor)) {
        pclose(processFileDescriptor);
        runStats = JSON::readFromFile(boxId);
        system(("rm " + boxId).c_str());
        runStatsAvailable = true;
        available = true;
    }
    return available;
}

pair<string, JSON> BoxWrapper::getRunStats() const {
    if (!runStatsAvailable) {
        throw runtime_error("Askex box for run stats, "
                            "but run stats are not available!");
    }
    return {processOutput, runStats};
}

BoxExecutor::BoxExecutor(int testIndexToRun, string binaryPath):
        Executor(testIndexToRun), box(new Box(argumentBoxId, binaryPath)) {}

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
    box->run(testIndex);
    while (!box->poll()) {}
    pair<string, JSON> boxRunStats = box->getRunStats();
    if ((int)boxRunStats.second["exitCode"] != 0) {
        test->setFailure(boxRunStats.first);
    }
}

}
